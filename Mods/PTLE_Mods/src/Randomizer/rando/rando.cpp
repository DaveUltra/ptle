#include "rando/rando.h"

#include "ptle/levels/level_info.h"

#include "utils/log.h"

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>


// Config.
uint32_t rando_seed = 0;
uint32_t starting_area = levels_t::FLOODED_COURTYARD;
bool legacy = true;



uint32_t rando_prev_area = starting_area;
std::map<Transition, Transition> rando_transitions_map;

std::set<Transition> disabledTransitions;

static const Transition ONE_WAY_TRANSITIONS[] = {
	// White Valley geyser.
	{ levels_t::WHITE_VALLEY, levels_t::MOUNTAIN_SLED_RUN },
	// Apu Illapu Shrine geyser.
	{ levels_t::APU_ILLAPU_SHRINE, levels_t::WHITE_VALLEY },
	// Apu Illapu Shrine one-way door.
	{ levels_t::MOUNTAIN_SLED_RUN, levels_t::APU_ILLAPU_SHRINE },
	// Jungle Canyon waterfall.
	{ levels_t::CAVERN_LAKE, levels_t::JUNGLE_CANYON }
};

// Putting levels in this list will make the randomizer ignore them as starting areas
// and not randomize them, if their transitions haven't been excluded yet.
std::set<uint32_t> excludedLevels;


static inline void disableTransition( uint32_t areaFromID, uint32_t areaToID )
{
	Transition t = { areaFromID, areaToID };
	disabledTransitions.emplace( t );
}
static inline void disableTransition2( uint32_t areaFromID, uint32_t areaToID )
{
	Transition t0 = { areaFromID, areaToID };
	Transition t1 = { areaToID, areaFromID };
	disabledTransitions.emplace( t0 );
	disabledTransitions.emplace( t1 );
}

static void set_excluded_levels()
{
	static const uint32_t excluded[] = {
		// Ignore crash site for now, as this causes losing all acquired upgrades & items.
		levels_t::CRASH_SITE,

		// Ignore native games.
		levels_t::KABOOM,
		levels_t::PICKAXE_RACE,
		levels_t::RAFT_BOWLING,
		levels_t::TUCO_SHOOT,
		levels_t::WHACK_A_TUCO,

		// Avoid warping to spirit fights without warning.
		levels_t::MONKEY_SPIRIT,
		levels_t::SCORPION_SPIRIT,
		levels_t::PENGUIN_SPIRIT,

		// Skip viracocha & plane crash cutscenes.
		levels_t::PLANE_CUTSCENE,
		levels_t::VIRACOCHA_MONOLITHS_CUTSCENE,

		// Teleports don't work, so just straight up remove them.
		levels_t::TELEPORT,

		// Don't warp straight to bosses.
		levels_t::RUINS_OF_EL_DORADO,
		levels_t::ANCIENT_EL_DORADO,
	};

	// Dare tell me the STL isn't disgusting.
	excludedLevels.insert( excluded, excluded + _countof(excluded) );
}

static void set_disabled_transitions()
{
	// The 3 Spirit Fights are not randomized,
	// because that causes issues with the transformation cutscene trigger.
	// Plus it wouldn't really improve anything, given that the Temples are randomized anyway.
	disableTransition2( levels_t::MONKEY_TEMPLE, levels_t::MONKEY_SPIRIT );
	disableTransition2( levels_t::SCORPION_TEMPLE, levels_t::SCORPION_SPIRIT );
	disableTransition2( levels_t::PENGUIN_TEMPLE, levels_t::PENGUIN_SPIRIT );
	// The 5 Native Games are currently chosen to not be randomized.
	// If we at some point decide to randomize them anyway we'll have to do some rigorous testing,
	// because it's very much possible this will cause some bugs.
	disableTransition2( levels_t::NATIVE_VILLAGE, levels_t::WHACK_A_TUCO );
	disableTransition2( levels_t::NATIVE_VILLAGE, levels_t::TUCO_SHOOT );
	disableTransition2( levels_t::NATIVE_VILLAGE, levels_t::RAFT_BOWLING );
	disableTransition2( levels_t::NATIVE_VILLAGE, levels_t::PICKAXE_RACE );
	disableTransition2( levels_t::NATIVE_VILLAGE, levels_t::KABOOM );
	// The Endgame transitions are not randomized.
	// Currently there are no plans to randomize these transitions.
	disableTransition2( levels_t::ST_CLAIRE_EXCAVATION_CAMP_DAY, levels_t::ST_CLAIRE_EXCAVATION_CAMP_NIGHT );
	disableTransition( levels_t::GATES_OF_EL_DORADO, levels_t::RUINS_OF_EL_DORADO );
	disableTransition( levels_t::RUINS_OF_EL_DORADO, levels_t::ANCIENT_EL_DORADO );
	disableTransition( levels_t::ANCIENT_EL_DORADO, levels_t::GATES_OF_EL_DORADO );
	// The Unused Beta Volcano Level is not randomized yet,
	// but this can absolutely be randomized later at some point.
	// It will require some special code though.
	disableTransition( levels_t::BETA_VOLCANO, levels_t::JUNGLE_CANYON );
	disableTransition( levels_t::BETA_VOLCANO, levels_t::PLANE_COCKPIT );
}

bool is_area_excluded( uint32_t levelID )
{
	return excludedLevels.find( levelID ) != excludedLevels.end();
}

bool is_transition_disabled( const Transition& t )
{
	return disabledTransitions.find( t ) != disabledTransitions.end();
}



static uint32_t parse_hex( const std::string& s )
{
	uint32_t v = 0;
	for ( char c : s ) {
		v *= 16;
		if ( c >= '0' && c <= '9' ) {
			v += (c - '0');
		}
		else if ( c >= 'a' && c <= 'f' ) {
			v += (c - 'a' + 10);
		}
		else if ( c >= 'A' && c <= 'F' ) {
			v += (c - 'A' + 10);
		}
	}
	return v;
}

static void set_random_config()
{
	rando_seed = time(0);

	srand( rando_seed );

	starting_area = levels_t::FLOODED_COURTYARD;
	legacy = true;
}

void load_config()
{
	set_random_config();

	std::ifstream cfg( "cfg/Randomizer/config.txt" );

	std::string line;
	std::getline(cfg, line);
	if ( line != "[RandomizerPC]" ) return;

	while ( std::getline(cfg, line) ) {
		size_t eq = line.find( '=' );
		if ( line.empty() || eq == std::string::npos ) {
			continue;
		}

		std::string option = line.substr( 0, eq );
		std::string value = line.substr( eq + 1 );
		if ( value.empty() ) {
			continue;
		}

		if ( option == "legacy" ) {
			legacy = (value == "true");
		}
		else if ( option == "seed" ) {
			rando_seed = atoi( value.c_str() );
			srand( rando_seed );
		}
		else if ( option == "startingArea" ) {
			starting_area = level_get_by_crc( parse_hex( value ) );
		}
	}

	cfg.close();
}



// in transition_infos.cpp.
bool load_transition_infos();

void rando_init()
{
	if ( !load_transition_infos() ) {
		return;
	}

	load_config();

	set_excluded_levels();
	set_disabled_transitions();

	log_printf( "Rando config :\n" );
	log_printf( "- Legacy : %s\n", legacy ? "true" : "false" );
	log_printf( "- Seed : %d\n", rando_seed );
	log_printf( "- Starting area : %s (0x%.8X)\n", level_get_name(starting_area), level_get_crc(starting_area) );
}

static void generate_linked_transitions()
{

}

static int get_random_one_way_transition( const Transition& original, std::vector<Transition>& possibleRedirections )
{
	if ( possibleRedirections.size() <= 1 ) {
		return -1;
	}

	int choice = rand() % (possibleRedirections.size() - 1);
	if ( original.areaFromID == possibleRedirections[choice].areaToID ) {
		choice = possibleRedirections.size() - 1;
	}

	return choice;
}

static void generate_legacy()
{
	std::vector<Transition> possibleRedirections;

	for ( auto& p : transition_infos ) {
		for ( const Area& area : p.second ) {
			uint32_t levelFrom = level_get_by_crc( area.areaID );
			if ( is_area_excluded(levelFrom) ) continue;

			for ( const Exit& exit : area.exits ) {
				uint32_t levelTo = level_get_by_crc( exit.areaID );
				if ( is_area_excluded(levelTo) ) continue;

				Transition t = { levelFrom, levelTo };
				possibleRedirections.push_back( t );
			}
		}
	}

	for ( auto& p : transition_infos ) {
		for ( const Area& area : p.second ) {
			uint32_t levelFrom = level_get_by_crc( area.areaID );
			if ( is_area_excluded(levelFrom) ) continue;

			for ( const Exit& exit : area.exits ) {
				uint32_t levelTo = level_get_by_crc( exit.areaID );
				if ( is_area_excluded(levelTo) ) continue;

				Transition original = { levelFrom, levelTo };
				if ( is_transition_disabled(original) ) {
					continue;
				}

				int redirect = get_random_one_way_transition( original, possibleRedirections );
				if ( redirect != -1 ) {
					Transition r = { original.areaFromID, possibleRedirections[redirect].areaToID };
					rando_transitions_map.insert( std::pair<Transition, Transition>(original, r) );
					possibleRedirections.erase( possibleRedirections.begin() + redirect );
				}
			}
		}
	}
}

void generate_randomized_map()
{
	if ( legacy ) {
		generate_legacy();
	}
	else {
		generate_linked_transitions();
	}
	write_graphml();
}
