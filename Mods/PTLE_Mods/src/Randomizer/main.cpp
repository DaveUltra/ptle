/*
 * Pitfall Randomizer for PC.
 * Original mod by Avasam (@Avasam) and creepy koala (@wossnameGitHub).
 *
 * -----
 *
 * FEATURES & IMPLEMENTATION
 *
 * Entrance Randomizer :
 * - The mod injects a function to execute upon hitting a level transition. That way,
 *   the destination level can be picked up to determine which way we're going, and
 *   modified to force the game to load any area of our choice. Furthermore, the
 *   previous area value can be changed to control which exit we're coming out of.
 *
 * Shaman Shop :
 * - Optionally, shaman prices can be randomized. Currently, prices are generated on
 *   the mod's initialization, and the prices are applied to the actual game on every
 *   level load.
 *
 * Item Swap :
 * - For convenience, and because many story points mess with this ability, the mod
 *   ensures that item swap is always enabled. This prevents the player from losing
 *   that ability after completing Monkey Temple, for example.
 *   Right now, item swap is enforced at every level load, except the first one.
 *
 * Softlock Prevention :
 * - There are cases where an entrance will lead Harry to run against a wall indefinitely,
 *   such as St.Claire -> Flooded Courtyard, Twin Outposts -> Flooded Courtyard...
 *   To prevent this, we force the game to give control of Harry back to the player.
 *   Apparently, setting Harry high enough into the air causes exactly that to happen.
 *
 * Graph Generation :
 * - On mod init, a .graphml file is generated showing the randomized map, with some
 *   additional information. The file can be viewed on graphonline.ru/en, which is
 *   a very simple online graph editor.
 */


#include <Windows.h>

#include "ptle/types/types.h"
#include "ptle/levels/level_info.h"

#include "ptle/EIBeast.h"
#include "ptle/EScriptContext.h"

#include "injector/injector.hpp"
#include "utils/log.h"
#include "utils/func.h"

#include "rando/rando.h"



void ensure_item_swap()
{
	*((bool*) 0x9106A2) = false;
}


GET_METHOD( 0x5EBA90, void, ScheduleWorldLoad, void*, uint32_t, bool );

void hijack_transition( void* globalStruct, uint32_t levelCRC, bool p2 )
{
	extern uint32_t rando_prev_area;

	ensure_item_swap();

	if ( rando_config.randomizeShamanShop ) {
		patch_shaman_shop();
	}

	uint32_t* prevAreaPtr = find_previous_area_memory();
	if ( !prevAreaPtr ) {
		log_printf( "FATAL : Could not retrieve previous area!\n" );
		return;
	}

	uint32_t* currentAreaCRC = ((uint32_t*) 0x917088);
	uint32_t targetAreaID = level_get_by_crc( levelCRC );
	uint32_t prevAreaID = level_get_by_crc( *currentAreaCRC );//level_get_by_crc( *prevAreaPtr );

	// Update previous area.
	rando_prev_area = prevAreaID;

	// Transition in progress.
	Transition original( rando_prev_area, targetAreaID );
	log_printf( "Game wants to go from \"%s\" (0x%X) to \"%s\" (0x%X).\n",
		level_get_name(original.areaFromID), level_get_crc(original.areaFromID),
		level_get_name(original.areaToID), level_get_crc(original.areaToID) );

	// Override transition.
	Transition redirect = original;
	if ( rando_map.spoofTransition(redirect) ) {
		log_printf( "Redirection is \"%s\" (0x%X) to \"%s\" (0x%X).\n",
			level_get_name(redirect.areaFromID), level_get_crc(redirect.areaFromID),
			level_get_name(redirect.areaToID), level_get_crc(redirect.areaToID) );

		extern Transition rando_redirect_transition;
		rando_redirect_transition = redirect;

		// Legacy has to patch teleporters to work. For now, the teleporter transitions are not randomized, so no need.
		if ( rando_config.legacy ) {
			// *prevAreaPtr = level_get_crc( levels_t::TURTLE_MONUMENT );
		}
		else {
			// Set previous area for correct transition (linked transitions mode).
			*prevAreaPtr = level_get_crc( redirect.areaFromID );
			levelCRC = level_get_crc( redirect.areaToID );
		}
	}
	else {
		log_printf( "Transition has no override, we'll proceed like normal.\n" );
	}

	log_printf( "------------------------------\n" );

	//log_printf( "Prev : \"%s\" (0x%X)\n", level_get_name(level_get_by_crc(*prevAreaPtr)), *prevAreaPtr );
	ScheduleWorldLoad( globalStruct, levelCRC, p2 );
	//log_printf( "Prev : \"%s\" (0x%X)\n", level_get_name(level_get_by_crc(*prevAreaPtr)), *prevAreaPtr );
}
MAKE_THISCALL_WRAPPER( hijack_transition_ptr, hijack_transition );



// Monkey Temple crash fix.
// TODO : MAKES MOLE FIRE AT YOU :/
GET_FUNC( 0x422C30, void, Script_SetBeastTarget_Original, EScriptContext* );
GET_METHOD( 0x6C2E70, void*, EScriptContext_PopVar, EScriptContext* );
void Script_SetBeastTarget_Safe( EScriptContext* context )
{
	EIBeast* beast = type_cast<EIBeast>( context->m_owningInstance, get_type_by_vtable(0x86C3D0) );
	if ( !beast ) {
		EScriptContext_PopVar( context );
		return;
	}

	Script_SetBeastTarget_Original( context );
}



// Display randomized level name
// when approaching a level transition.
char* get_patched_level_name( char* orig )
{
	static char buf[128];

	// Get redirect.
	uint32_t currentAreaCRC = *((uint32_t*) 0x917088);
	uint32_t destCRC = level_HashString( orig );

	log_printf( "Patching dialog string \"%s\" (0x%.8X)\n", orig, destCRC );

	// Handle levels that have spcial names.
	switch ( destCRC )
	{
	case 0x6D9360AB:  destCRC = level_get_crc(levels_t::TELEPORT); break;  // "warp_b"
	case 0x07234059:  destCRC = level_get_crc(levels_t::TELEPORT); break;  // "warp_s"
	case 0x1A94503D:  destCRC = level_get_crc(levels_t::TELEPORT); break;  // "warp_c"
	}

	Transition tr( level_get_by_crc(currentAreaCRC), level_get_by_crc(destCRC) );

	rando_map.spoofTransition( tr );
	uint32_t redirectID = tr.areaToID;

	// Compose transition string.
	const char* areaName = level_get_info(redirectID)->assetName;

	buf[0] = 0;
	strcat( buf, "enter " );
	strcat( buf, areaName );

	return buf;
}

GET_FUNC( 0x5412D0, void, Script_QueueDialogHint_Original, EScriptContext* );
void Script_QueueDialogHint_hijack( EScriptContext* context )
{
	int* var = (int*) context->m_scriptStack[context->m_stackPointer - 1];
	char** dialogHintStr = (char**) var+2;

	// "To <area_name>" hint.
	if ( strncmp( "enter ", *dialogHintStr, 6 ) == 0 ) {
		char* origStr = *dialogHintStr;
		*dialogHintStr = get_patched_level_name( *dialogHintStr + 6 );

		Script_QueueDialogHint_Original( context );

		*dialogHintStr = origStr;
	}
	else {
		Script_QueueDialogHint_Original( context );
	}
}



void InitMod()
{
	log_printf( "PTLE Randomizer : Starting...\n" );

	rando_init();

	rando_map.generateMap();

	if ( rando_config.randomizeShamanShop ) {
		randomize_shaman_shop();
	}

	// TODO : Load world call (new game).

	// Load world call (level transition).
	injector::MakeCALL( 0x5ECC70, hijack_transition_ptr );

	// TODO : What happens when you die????

	// Hijack new game starting area.
	uint32_t startCRC = level_get_crc( rando_config.startingArea );
	injector::WriteMemory( 0x5EB9E6, startCRC );

	// End of level load routine, we clear an otherwise unused debug log function call
	// and place our Harry up-teleport function on top of it.
	injector::MakeNOP( 0x5EC167, 13 );
	injector::MakeCALL( 0x5EC167, prevent_transition_softlock );

	// Bonus : Replace this script function with one that null-checks first. This protects against Monkey Temple crash.
	injector::WriteMemory( 0x8F0A0C, &Script_SetBeastTarget_Safe );

	// Patch this script function, to display the actual (randomized) destination level name when approaching
	// an entrance.
	injector::WriteMemory( 0x8F04EC, &Script_QueueDialogHint_hijack );

	log_printf( "PTLE Randomizer : Mod started.\n" );
}



BOOL WINAPI DllMain( HINSTANCE hinstace, DWORD reason, LPVOID )
{
	switch ( reason )
	{
	case DLL_PROCESS_ATTACH:
		InitMod();
		break;
	default:
		break;
	}

	return true;
}
