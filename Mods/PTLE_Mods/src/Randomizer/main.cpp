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


#include "gizmod/Gizmod.h"
#include "gizmod/GizmodPlugin.h"
#include "gizmod/event/LoadLevelEvent.h"
#include "gizmod/event/LevelLoadedEvent.h"
#include "gizmod/event/ShamanPurchaseEvent.h"

#include "ptle/types/types.h"
#include "ptle/levels/level_info.h"

#include "ptle/EIBeast.h"
#include "ptle/EScriptContext.h"

#include "injector/injector.hpp"
#include "utils/log.h"
#include "utils/func.h"

#include "rando/rando.h"
#include "rando/item_rando.h"
#include "rando/menu_patcher.h"

#include <fstream>



void ensure_item_swap()
{
	*((bool*) 0x9106A2) = false;
}


void hijack_transition( LoadLevelEvent& event )
{
	uint32_t targetAreaCRC = event.getLevelCRC();
	uint32_t prevAreaCRC = event.getPrevCRC();
	const uint32_t currentAreaCRC = Gizmod::getCurrentLevelCRC();

	const uint32_t targetAreaID = level_get_by_crc( event.getLevelCRC() );
	const uint32_t prevAreaID = level_get_by_crc( prevAreaCRC );


	switch ( prevAreaCRC )
	{
	case 0x402D3708: prevAreaCRC = levelCRC::FLOODED_COURTYARD; break;  // Flooded Courtyard to Jungle Trail (to cave).
	case 0x83A6748F: prevAreaCRC = levelCRC::JUNGLE_TRAIL;      break;  // Jungle Trail to Flooded Courtyard (by ice wall).
	//case 0xF3ACDE92:   // Apu Illapu to White Valley.
	}

	// Altar of Ages cutscene redirects to (Mysterious Temple -> BB Camp).
	// But technically, we're going back from Altar.
	if ( prevAreaCRC == levelCRC::MYSTERIOUS_TEMPLE && targetAreaCRC == levelCRC::BITTENBINDER_CAMP && currentAreaCRC == levelCRC::ALTAR_OF_AGES ) {
		prevAreaCRC = levelCRC::ALTAR_OF_AGES;
		targetAreaCRC = levelCRC::MYSTERIOUS_TEMPLE;
		// Fall through to spoof_transition().
	}


	// Detect spirit temple completion.
	if ( prevAreaCRC == levelCRC::MONKEY_SPIRIT && targetAreaCRC == levelCRC::MONKEY_TEMPLE ) {
		completed_monkey_temple = true;
	}
	else if ( prevAreaCRC == levelCRC::SCORPION_SPIRIT && targetAreaCRC == levelCRC::SCORPION_TEMPLE ) {
		completed_scorpion_temple = true;
	}
	else if ( prevAreaCRC == levelCRC::PENGUIN_SPIRIT && targetAreaCRC == levelCRC::PENGUIN_TEMPLE ) {
		completed_penguin_temple = true;
	}


	// Transition in progress.
	Transition original( prevAreaCRC, targetAreaCRC );
	Gizmod::getInstance()->getLogger()->log_printf( "Game wants to go from \"%s\" (0x%X) to \"%s\" (0x%X).\n",
		level_get_name(level_get_by_crc(original.areaFromID)), original.areaFromID,
		level_get_name(level_get_by_crc(original.areaToID)), original.areaToID );


	// Override transition.
	extern Transition rando_redirect_transition;
	Transition redirect = original;
	if ( rando_map.spoofTransition(redirect) ) {
		Gizmod::getInstance()->getLogger()->log_printf( "Redirection is \"%s\" (0x%X) to \"%s\" (0x%X).\n",
			level_get_name(level_get_by_crc(redirect.areaFromID)), redirect.areaFromID,
			level_get_name(level_get_by_crc(redirect.areaToID)), redirect.areaToID );

		rando_redirect_transition = redirect;

		// Legacy has to patch teleporters to work. For now, the teleporter transitions are not randomized, so no need.
		if ( rando_config.legacy ) {
			// *prevAreaPtr = level_get_crc( levels_t::TURTLE_MONUMENT );
		}
		else {
			// Special level transitions.
			/*if ( prevAreaID == levels_t::TWIN_OUTPOSTS_UNDERWATER ) { // Was in Underwater.
				switch ( *prevAreaPtr )
				{
				case TwinOutpostsWaterExits::WATER_TO_JUNGLE:

					break;
				case TwinOutpostsWaterExits::WATER_TO_BURNING:
					break;
				}
			}
			else if ( prevAreaID == levels_t::TWIN_OUTPOSTS ) {  // Was in Twin Outposts.
				switch ( *prevAreaPtr )
				{
				case TwinOutpostsWaterExits::JUNGLE_TO_WATER: break;
				case TwinOutpostsWaterExits::BURNING_TO_WATER: break;
				}
			}*/
			/*else if ( redirect.areaToID == levels_t::TWIN_OUTPOSTS ) {  // Going to Twin Outposts.
				if ( redirect.areaFromID == levels_t::TWIN_OUTPOSTS_UNDERWATER ) {
					*prevAreaPtr = TwinOutpostsWaterExits::WATER_TO_JUNGLE;
				}
			}
			else if ( redirect.areaToID == levels_t::TWIN_OUTPOSTS_UNDERWATER ) {

			}*/
			{
				// Set previous area for correct transition (linked transitions mode).
				prevAreaCRC = redirect.areaFromID;
				targetAreaCRC = redirect.areaToID;
			}
		}
	}
	else {
		rando_redirect_transition = redirect;

		Gizmod::getInstance()->getLogger()->log( "Transition has no override, we'll proceed like normal.\n" );
	}

	Gizmod::getInstance()->getLogger()->log( "------------------------------\n" );

	// Update load level request with patched values.
	// Note : Patching over the prevAreaCRC value will ensure that the player emerges from the correct entrance.
	event.setLevelCRC( targetAreaCRC );
	event.setPrevCRC( prevAreaCRC );
}

MAKE_THISCALL_WRAPPER( hijack_transition_ptr, hijack_transition );



void read_transition( void* globalStruct, uint32_t targetAreaCRC, bool p2 )
{
	uint32_t* prevAreaPtr = find_previous_area_memory();
	if ( !prevAreaPtr ) {
		log_printf( "FATAL : Could not retrieve previous area!\n" );
		return;
	}

	uint32_t* currentAreaPtr = ((uint32_t*) 0x917088);
	uint32_t currentAreaCRC = *currentAreaPtr;
	uint32_t prevAreaCRC = *prevAreaPtr;
	uint32_t targetAreaID = level_get_by_crc( targetAreaCRC );
	uint32_t prevAreaID = level_get_by_crc( currentAreaCRC );

	if ( prevAreaCRC == currentAreaCRC ) {
		log_printf( "Prev and Current match!\n" );
	}
	else {
		log_printf( "Prev and Current DO NOT match!\n" );
	}

	// Transition in progress.
	Transition original( prevAreaCRC, targetAreaCRC );
	log_printf( "Game wants to go from \"%s\" (0x%X) to \"%s\" (0x%X).\n",
		level_get_name(level_get_by_crc(original.areaFromID)), original.areaFromID,
		level_get_name(level_get_by_crc(original.areaToID)), original.areaToID );


	log_printf( "Prev : \"%s\" (0x%X)\n", level_get_name(level_get_by_crc(prevAreaCRC)), prevAreaCRC );

	log_printf( "------------------------------\n" );
}

MAKE_THISCALL_WRAPPER( read_transition_ptr, read_transition );



// Display randomized level name
// when approaching a level transition.
char* get_patched_level_name( char* orig )
{
	static char buf[128];
	char* levelName = orig + 6;  // Omit "enter ".

	// Get redirect.
	uint32_t currentAreaCRC = *((uint32_t*) 0x917088);
	uint32_t destCRC = level_HashString( levelName );

	Transition tr;
	tr.areaFromID = currentAreaCRC;

	// Handle levels that have special names.
	switch ( destCRC )
	{
	case 0x6D9360AB:  // "warp_b"
	case 0x07234059:  // "warp_s"
	case 0x1A94503D:  // "warp_c"
		tr.areaToID = levelCRC::TELEPORT;
		break;

	case 0xE6B9138A:  // "b52"  (Twin Outposts)
	case 0x4EED5037:  // "b52n" (Jungle Outpost)    | These two are only used
	case 0x2DEB3CEE:  // "b52s" (Burning Outpost)   | in the underwater passage.
		tr.areaToID = 0xE6B9138A;
		break;

	default:
		tr.areaToID = destCRC;
		break;
	}

	// Failed.
	if ( !rando_map.spoofTransition( tr ) ) {
		return orig;
	}

	// Areas that do not have a name.
	switch ( tr.areaToID )
	{
	case levelCRC::MONKEY_SPIRIT:   tr.areaToID = levelCRC::MONKEY_TEMPLE;   break;
	case levelCRC::SCORPION_SPIRIT: tr.areaToID = levelCRC::SCORPION_TEMPLE; break;
	case levelCRC::PENGUIN_SPIRIT:  tr.areaToID = levelCRC::PENGUIN_TEMPLE;  break;
	}

	// Compose transition string.
	const char* areaName = level_get_info( level_get_by_crc(tr.areaToID) )->assetName;

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
	if ( strncmp("enter ", *dialogHintStr, 6) == 0 ) {
		char* origStr = *dialogHintStr;
		*dialogHintStr = get_patched_level_name( *dialogHintStr );

		Script_QueueDialogHint_Original( context );

		*dialogHintStr = origStr;
	}
	else {
		Script_QueueDialogHint_Original( context );
	}
}


void generate_spoiler_logs( std::ostream& os )
{
	log_shaman_shop( os );
	log_item_rando( os );
}



class RandomizerPlugin : public GizmodPlugin
	, public ILoadLevelListener
	, public ILevelLoadedListener
	, public IShamanPurchaseListener
{
public:

	virtual const char* getDisplayName() const override { return "PTLE Randomizer"; }
	virtual const char* getSystemName() const override { return "Randomizer"; }

	// Loading a level.
	virtual void onLoadLevel( LoadLevelEvent& event ) override
	{
		if ( rando_config.entranceRando ) {
			hijack_transition( event );
		}
		//read_transition( event );
	}

	// Level finished loading.
	virtual void onLevelLoaded( LevelLoadedEvent& event ) override
	{
		ensure_item_swap();

		if ( rando_config.randomizeShamanShop ) {
			patch_shaman_shop();
		}

		prevent_transition_softlock();
	}

	// Purchasing a shaman item.
	virtual void onShamanPurchase( ShamanPurchaseEvent& event ) override
	{
		const char* name = 0;
		switch ( event.getItem() )
		{
		case ShamanShop::HEALTH1: name = "health upgrade"; break;
		case ShamanShop::CANTEEN1: name = "canteen upgrade"; break;
		case ShamanShop::SMASH_STRIKE: name = "smash strike"; break;
		case ShamanShop::SUPER_SLING: name = "super sling"; break;
		case ShamanShop::BREAKDANCE: name = "breakdance"; break;
		case ShamanShop::JUNGLE_NOTES: name = "jungle notes"; break;
		case ShamanShop::NATIVE_NOTES: name = "native notes"; break;
		case ShamanShop::CAVERN_NOTES: name = "cavern notes"; break;
		case ShamanShop::MOUTAIN_NOTES: name = "mountain notes"; break;
		case ShamanShop::MYSTERY_ITEM: name = "mystery item"; break;
		}

		Gizmod::getInstance()->getLogger()->log_printf( "%s %s.\n", (event.hasEnoughIdols() ? "Purchased" : "Not enough idols for"), name );
	}

	virtual void onEnable()
	{
		register_types();

		rando_init();

		if ( rando_config.entranceRando ) {
			rando_map.generateMap();
		}

		if ( rando_config.randomizeShamanShop ) {
			randomize_shaman_shop();
		}

		// TODO : Load world call (new game).

		EventListener* eventListener = Gizmod::getInstance()->getEventListener();
		eventListener->registerEvent<LoadLevelEvent>( this );
		eventListener->registerEvent<LevelLoadedEvent>( this );
		eventListener->registerEvent<ShamanPurchaseEvent>( this );

		// TODO : What happens when you die????

		// Hijack new game starting area.
		uint32_t startCRC = rando_config.startingArea;
		injector::WriteMemory( 0x5EB9E6, startCRC );

		// Patch this script function, to display the actual (randomized) destination level name when approaching
		// an entrance.
		if ( rando_config.entranceRando ) {
			injector::WriteMemory( 0x8F04EC, &Script_QueueDialogHint_hijack );
		}

		// Item rando.
		item_rando_init();

		// Generate graph.
		write_graphml();


		// Menu patcher. Replaces "NEW GAME".
		init_menu_patcher();


		Gizmod::getInstance()->getLogger()->log( "Generating spoiler logs...\n" );
		std::wstring cfgDir = this->getConfigDirectory();
		std::ofstream spoilerLogFile( cfgDir + L"/spoiler.log" );
		generate_spoiler_logs( spoilerLogFile );
	}
};

DECLARE_PLUGIN( RandomizerPlugin );
