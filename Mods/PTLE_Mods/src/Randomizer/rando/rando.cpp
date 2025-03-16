#include "rando/rando.h"

#include "ptle/levels/level_info.h"
#include "ptle/containers/TreeMap/TreeMap.h"

#include "utils/log.h"

#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>


// Config.
RandoConfig::RandoConfig()
	: seed( 0 )
	, startingArea( levels_t::FLOODED_COURTYARD )
	, legacy( true )
	, randomizeShamanShop( true )
	, skipJaguar2( false )
	, skipWaterLevels( false )
	, immediateSpiritFights( false )
{

}

RandoConfig rando_config;



uint32_t rando_prev_area = -1;
std::map<Transition, Transition> rando_transitions_map;

std::set<Transition> disabledTransitions;

static const Transition ONE_WAY_TRANSITIONS[] = {
	// White Valley geyser.
	Transition( levels_t::WHITE_VALLEY, levels_t::MOUNTAIN_SLED_RUN ),
	// Apu Illapu Shrine geyser.
	Transition( levels_t::APU_ILLAPU_SHRINE, levels_t::WHITE_VALLEY ),
	// Apu Illapu Shrine one-way door.
	Transition( levels_t::MOUNTAIN_SLED_RUN, levels_t::APU_ILLAPU_SHRINE ),
	// Jungle Canyon waterfall.
	Transition( levels_t::CAVERN_LAKE, levels_t::JUNGLE_CANYON )
};

// Putting levels in this list will make the randomizer ignore them as starting areas
// and not randomize them, if their transitions haven't been excluded yet.
std::set<uint32_t> excludedLevels;


static inline void disableTransition( uint32_t areaFromID, uint32_t areaToID )
{
	Transition t( areaFromID, areaToID );
	disabledTransitions.emplace( t );
}
static inline void disableTransition2( uint32_t areaFromID, uint32_t areaToID )
{
	Transition t0( areaFromID, areaToID );
	Transition t1( areaToID, areaFromID );
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

void load_config()
{
	bool overridenStartArea = false;

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
			rando_config.legacy = (value == "true");
		}
		else if ( option == "seed" ) {
			rando_config.seed = atoi( value.c_str() );
			srand( rando_config.seed );
		}
		else if ( option == "startingArea" ) {
			uint32_t areaID = level_get_by_crc( parse_hex(value) );
			if ( areaID != -1 ) {
				rando_config.startingArea = areaID;
				overridenStartArea = true;
			}
		}
		else if ( option == "randomizeShamanShop" ) {
			rando_config.randomizeShamanShop = (value == "true");
		}
		else if ( option == "skipJaguar2" ) {
			rando_config.skipJaguar2 = (value == "true");
		}
		else if ( option == "skipWaterLevels" ) {
			rando_config.skipWaterLevels = (value == "true");
		}
		else if ( option == "immediateSpiritFights" ) {
			rando_config.immediateSpiritFights = (value == "true");
		}
	}

	cfg.close();
}



// in transition_infos.cpp.
bool load_transition_infos();

static inline const char* bool_to_str(bool b)
{
	return b ? "true" : "false";
}

void rando_init()
{
	if ( !load_transition_infos() ) {
		return;
	}

	load_config();

	rando_prev_area = rando_config.startingArea;

	set_excluded_levels();
	set_disabled_transitions();

	log_printf( "Rando config :\n" );
	log_printf( "- Legacy : %s\n", bool_to_str(rando_config.legacy) );
	log_printf( "- Seed : %d\n", rando_config.seed );
	log_printf( "- Starting area : %s (0x%.8X)\n", level_get_name(rando_config.startingArea), level_get_crc(rando_config.startingArea) );
	log_printf( "- Randomize shaman shop : %s\n",   bool_to_str(rando_config.randomizeShamanShop) );
	log_printf( "- Skip Jaguar 2 : %s\n",           bool_to_str(rando_config.skipJaguar2) );
	log_printf( "- Skip water levels : %s\n",       bool_to_str(rando_config.skipWaterLevels) );
	log_printf( "- Immediate spirit fights : %s\n", bool_to_str(rando_config.immediateSpiritFights) );
}

static Transition mirror( const Transition& t )
{
	return Transition( t.areaToID, t.areaFromID );
}

template<typename T>
static void vector_removeAt( std::vector<T>& v, int p )
{
	v.erase( v.begin() + p );
}

template<typename T>
static void vector_remove( std::vector<T>& v, const T& val )
{
	v.erase( std::remove(v.begin(), v.end(), val) );
}

static void generate_linked_transitions()
{
	std::set<Transition> processedTransitions;
	std::set<Transition> transitionsOneWaySet;
	std::vector<Transition> transitionsTwoWay;
	std::vector<Transition> transitionsOneWay;
	std::set<uint32_t> deadEnds;

	std::set<uint32_t> knownAreas;

	std::vector<uint32_t> availableAreas;
	std::map<uint32_t, std::vector<uint32_t>> availablePorts;

	// Sorting.
	// We list all accessible areas, as well as all undirected transitions (transitions that can be crossed both ways).
	// We leave one-way transitions (geysers, cavern lake to jungle canyon, etc...) unrandomized for now.
	for ( auto& p : transition_infos ) {
		for ( const Area& area : p.second ) {
			uint32_t levelFrom = level_get_by_crc( area.areaID );
			if ( is_area_excluded(levelFrom) ) continue;

			if ( knownAreas.find(levelFrom) == knownAreas.end() ) {
				availableAreas.push_back( levelFrom );
				knownAreas.emplace( levelFrom );
			}

			for ( const Exit& exit : area.exits ) {
				uint32_t levelTo = level_get_by_crc( exit.areaID );
				Transition actual( levelFrom, levelTo );
				Transition reverse = actual.mirror();

				if ( is_area_excluded(levelTo) ) continue;
				if ( is_transition_disabled(actual) || is_transition_disabled(reverse) ) continue;

				if ( knownAreas.find(levelTo) == knownAreas.end() ) {
					availableAreas.push_back( levelTo );
					knownAreas.emplace( levelTo );
				}

				// Promote to two-way.
				if ( processedTransitions.find(reverse) != processedTransitions.end() ) {
					transitionsOneWaySet.erase(reverse);
					transitionsTwoWay.push_back( actual );

					availablePorts[levelFrom].push_back( levelTo );
					availablePorts[levelTo].push_back( levelFrom );
				}
				else {
					processedTransitions.emplace( actual );
					transitionsOneWaySet.emplace( actual );
				}
			}

			if ( availablePorts[levelFrom].size() == 1 ) {
				deadEnds.emplace( levelFrom );
			}
		}
	}
	// TODO : That might not be necessary. The sorting process successfully picks up all 5 one-way exits of the game, which we already know about.
	for ( Transition t : transitionsOneWaySet ) {
		transitionsOneWay.push_back( t );
	}

	// Get rid of any area with no exits to process.
	std::vector<uint32_t> purgeList;
	for ( auto& p : availablePorts ) {
		if ( p.second.empty() ) {
			purgeList.push_back( p.first );
			continue;
		}
	}
	for ( uint32_t i : purgeList ) {
		availableAreas.erase( std::remove( availableAreas.begin(), availableAreas.end(), i ) );
		availablePorts.erase( i );
	}

	// Process.
	while ( !availableAreas.empty() ) {
		int levelFrom = rand() % availableAreas.size();
		int levelTo = rand() % availableAreas.size();

		if ( availableAreas.size() == 1 ) {
			log_printf( "This area wasn't processed correctly : %s\n", level_get_name(availableAreas[0]) );
			return;
		}

		// Prevent level from looping onto itself.
		if ( levelFrom == levelTo ) {
			if ( levelFrom == 0 ) levelTo++;
			else levelFrom--;
		}

		uint32_t levelFromID = availableAreas[levelFrom];
		uint32_t levelToID = availableAreas[levelTo];

		int fromExit = 0;
		int toEntrance = 0;

		// Form linked transition.
		Transition original( levelFromID, availablePorts[levelFromID][fromExit] );
		Transition redirect( availablePorts[levelToID][toEntrance], levelToID );
		rando_transitions_map[original] = redirect;
		rando_transitions_map[mirror(redirect)] = mirror(original);

		// Mark exits as used.
		vector_removeAt( availablePorts[levelFromID], fromExit );
		vector_removeAt( availablePorts[levelToID], toEntrance );

		// Remove filled up areas from available list.
		if ( availablePorts[levelFromID].empty() ) {
			availablePorts.erase( levelFromID );
			vector_remove( availableAreas, levelFromID );
		}
		if ( availablePorts[levelToID].empty() ) {
			availablePorts.erase( levelToID );
			vector_remove( availableAreas, levelToID );
		}
	}
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

				Transition t( levelFrom, levelTo );
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

				Transition original( levelFrom, levelTo );
				if ( is_transition_disabled(original) ) {
					continue;
				}

				int redirect = get_random_one_way_transition( original, possibleRedirections );
				if ( redirect != -1 ) {
					Transition r( original.areaFromID, possibleRedirections[redirect].areaToID );
					rando_transitions_map.insert( std::pair<Transition, Transition>(original, r) );
					possibleRedirections.erase( possibleRedirections.begin() + redirect );
				}
			}
		}
	}
}

void generate_randomized_map()
{
	if ( rando_config.legacy ) {
		generate_legacy();
	}
	else {
		generate_linked_transitions();
	}
	write_graphml();
}


uint32_t* find_previous_area_memory()
{
	// This tree map contains a node which points to the previous area value...
	TreeMap* map = ((TreeMap*) 0x91FE2C);

	// ... and this is the ID of said node.
	const uint32_t targetNodeID = 0x174CD628;

	// Look for it.
	// TODO : Binary search might be a good idea for performance.
	TreeMapNode* node = map->m_iterateFirst;
	while ( node && node->m_hash != targetNodeID ) {
		node = node->m_iterateNext;
	}

	if ( node ) {
		// This should be a pointer to and ESDInt.
		// Its value is at offset +0x08.
		uint32_t* scriptInt = (uint32_t*) node->m_ptr;
		return scriptInt + 2;
	}
	else {
		return 0;
	}
}


bool spoof_transition( Transition& o )
{
	// Where the actual hijacking takes place.
	Transition original, redirect;
	original = o;

	// Skip Viracocha cutscene entirely (then proceed with regular hijacking).
	if ( original.areaToID == levels_t::VIRACOCHA_MONOLITHS_CUTSCENE ) {
		original.areaToID = levels_t::VIRACOCHA_MONOLITHS;
	}

	auto it = rando_transitions_map.find( original );
	if ( it == rando_transitions_map.end() ) {
		return false;
	}
	else {
		o = it->second;
		return true;
	}
}
