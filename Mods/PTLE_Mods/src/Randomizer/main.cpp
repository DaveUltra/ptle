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

	uint32_t* currentAreaCRC = ((uint32_t*) 0x917088);
	uint32_t targetAreaID = level_get_by_crc( levelCRC );

	// Update previous area.
	rando_prev_area = level_get_by_crc( *currentAreaCRC );

	Transition original( rando_prev_area, targetAreaID );
	log_printf( "Game wants to go from \"%s\" (0x%X) to \"%s\" (0x%X).\n",
		level_get_name(original.areaFromID), level_get_crc(original.areaFromID),
		level_get_name(original.areaToID), level_get_crc(original.areaToID) );

	// Override transition.
	Transition redirect = original;
	if ( spoof_transition(redirect) ) {
		log_printf( "Redirection is \"%s\" (0x%X) to \"%s\" (0x%X).\n",
			level_get_name(redirect.areaFromID), level_get_crc(redirect.areaFromID),
			level_get_name(redirect.areaToID), level_get_crc(redirect.areaToID) );
	}
	else {
		log_printf( "Transition is ignored. We let the game proceed like normal.\n" );
	}

	extern Transition rando_redirect_transition;
	rando_redirect_transition = redirect;

	// Set previous area for correct transition.
	uint32_t* prevAreaPtr = find_previous_area_memory();
	if ( prevAreaPtr ) {
		uint32_t prevAreaCRC = *prevAreaPtr;
		uint32_t prevAreaID = level_get_by_crc(prevAreaCRC);
		log_printf( "Previous area : %s (0x%.8X)\n", level_get_name(prevAreaID), prevAreaCRC );

		// Legacy has to patch teleporters to work. For now, the teleporter transitions are not randomized, so no need.
		if ( rando_config.legacy ) {
			// *prevAreaPtr = level_get_crc( levels_t::TURTLE_MONUMENT );
		}
		else {
			*prevAreaPtr = level_get_crc( redirect.areaFromID );
		}
	}
	else {
		log_printf( "Could not retrieve previous area!\n" );
	}

	log_printf( "------------------------------\n" );


	uint32_t crc = level_get_crc( redirect.areaToID );
	ScheduleWorldLoad( globalStruct, crc, p2 );
}
MAKE_THISCALL_WRAPPER( hijack_transition_ptr, hijack_transition );


void InitMod()
{
	log_printf( "PTLE Randomizer : Starting...\n" );

	rando_init();

	generate_randomized_map();

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
