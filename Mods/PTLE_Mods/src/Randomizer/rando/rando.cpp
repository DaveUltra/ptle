#include "rando/rando.h"

#include "ptle/levels/level_info.h"
#include "ptle/containers/TreeMap/TreeMap.h"

#include "ptle/EIHarry.h"

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
RandoMap rando_map;

uint32_t rando_prev_area = -1;



std::set<Transition> disabledTransitions;

// These transitions can only be traversed in one direction.
static const Transition ONE_WAY_TRANSITIONS[] = {
	// White Valley geyser.
	Transition( levels_t::WHITE_VALLEY, levels_t::MOUNTAIN_SLED_RUN ),
	// Apu Illapu Shrine geyser.
	Transition( levels_t::APU_ILLAPU_SHRINE, levels_t::WHITE_VALLEY ),
	// Apu Illapu Shrine one-way door.
	Transition( levels_t::MOUNTAIN_SLED_RUN, levels_t::APU_ILLAPU_SHRINE ),
	// Jungle Canyon waterfall.
	Transition( levels_t::CAVERN_LAKE, levels_t::JUNGLE_CANYON ),
	// After Altar of Ages cutscene.
	Transition( levels_t::ALTAR_OF_AGES, levels_t::BITTENBINDER_CAMP ),
};

// These transitions can lead to softlocks due to closed doors / obstacles.
// Harry will run indefinitely against them and the game will never give us control back.
static const Transition SOFTLOCKABLE_TRANSITIONS[] = {
	Transition( levels_t::ST_CLAIRE_EXCAVATION_CAMP_DAY, levels_t::FLOODED_COURTYARD ),   // St.Claire wall (requires TNT).
	Transition( levels_t::TWIN_OUTPOSTS,                 levels_t::FLOODED_COURTYARD ),   // Monkey door.
	Transition( levels_t::SCORPION_TEMPLE,               levels_t::EYES_OF_DOOM ),        // Door to scorp temple.
	Transition( levels_t::MOUNTAIN_OVERLOOK,             levels_t::EYES_OF_DOOM ),        // Scorpion door.
	Transition( levels_t::EKKEKO_ICE_CAVERN,             levels_t::VALLEY_OF_SPIRITS ),   // Ice wall (requires TNT).
	Transition( levels_t::MOUNTAIN_SLED_RUN,             levels_t::COPACANTI_LAKE ),      // Three crystals door.
	Transition( levels_t::COPACANTI_LAKE,                levels_t::VALLEY_OF_SPIRITS ),   // Spirit door.
	Transition( levels_t::BATTERED_BRIDGE,               levels_t::ALTAR_OF_HUITACA ),    // Spider web (not a softlock, but it is a locked way).
};
const std::set<Transition> softlockableTransitions( SOFTLOCKABLE_TRANSITIONS, SOFTLOCKABLE_TRANSITIONS+_countof(SOFTLOCKABLE_TRANSITIONS) );

std::map<Transition, uint32_t> correctedTransitionsFrom;

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

static void set_corrected_transitions()
{
	// Dark cave connecting Jungle Trail to Flooded Courtyard.
	//correctedTransitionsFrom.emplace( Transition(levels_t::FLOODED_COURTYARD, levels_t::JUNGLE_TRAIL), 0x402D3708 );
	//correctedTransitionsFrom.emplace( Transition(levels_t::JUNGLE_TRAIL, levels_t::FLOODED_COURTYARD), 0x1AAF2535 );
	correctedTransitionsFrom.emplace( Transition(0x5CC8D550, levels_t::JUNGLE_TRAIL), 0x402D3708 );
	correctedTransitionsFrom.emplace( Transition(0x5CC8D550, levels_t::FLOODED_COURTYARD), 0x1AAF2535 ); /*0x5F29C550*/
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
	set_corrected_transitions();

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
	auto it = std::remove(v.begin(), v.end(), val);
	if ( it != v.end() )
	v.erase( it );
}



static uint32_t find_original_exit_to( uint32_t cur, uint32_t dst )
{
	for ( const Exit& exit : level_infos[cur]->exits ) {
		uint32_t destID = level_get_by_crc( exit.areaCRC );

		if ( rando_map.getTransitionsMap().at(Transition(cur, destID)).areaToID == dst ) {
			return destID;
		}
	}
	return -1;
}

static void remove_line_level( uint32_t level, uint32_t exitA, uint32_t exitB )
{
	// Get which zones have been mapped to the exits of the level we want to nuke.
	Transition normalTransitionA( level, exitA );
	Transition normalTransitionB( level, exitB );
	uint32_t levelA = rando_map.getTransitionsMap().at(normalTransitionA).areaToID;
	uint32_t levelB = rando_map.getTransitionsMap().at(normalTransitionB).areaToID;

	// Find which exit to take within those two levels to get to the removed level.
	uint32_t origExitA = find_original_exit_to( levelA, level );
	uint32_t origExitB = find_original_exit_to( levelB, level );

	// Link those exits together, therefore totally disconnecting the level in-between.
	rando_map.getTransitionsMap()[Transition(levelA, origExitA)] = Transition(origExitB, levelB);
	rando_map.getTransitionsMap()[Transition(levelB, origExitB)] = Transition(origExitA, levelA);
}

void RandoMap::generateLinkedTransitions()
{
	std::set<Transition> transitionsOneWay;
	std::set<Transition> transitionsTwoWay;
	std::set<uint32_t> deadEnds;

	std::set<uint32_t> knownAreas;                               // Set of all known areas.
	std::map<uint32_t, std::vector<uint32_t>> availablePorts;    // All remaining unconnected exits for each area.

	// Sorting.
	// We list all accessible areas, as well as all undirected transitions (transitions that can be crossed both ways).
	// We leave one-way transitions (geysers, cavern lake to jungle canyon, etc...) unrandomized for now.
	for ( auto& p : level_infos ) {
		uint32_t levelFrom = p.first;
		Area* area = p.second;
		if ( is_area_excluded(levelFrom) ) continue;

		if ( knownAreas.find(levelFrom) == knownAreas.end() ) {
			knownAreas.emplace( levelFrom );
		}

		for ( const Exit& exit : area->exits ) {
			uint32_t levelTo = level_get_by_crc( exit.areaCRC );
			Transition actual( levelFrom, levelTo );
			Transition reverse = actual.mirror();

			if ( is_area_excluded(levelTo) ) continue;
			if ( is_transition_disabled(actual) || is_transition_disabled(reverse) ) continue;

			if ( knownAreas.find(levelTo) == knownAreas.end() ) {
				knownAreas.emplace( levelTo );
			}

			// Promote to two-way.
			if ( transitionsOneWay.find(reverse) != transitionsOneWay.end() ) {
				transitionsOneWay.erase( reverse );
				transitionsTwoWay.emplace( actual );

				availablePorts[levelFrom].push_back( levelTo );
				availablePorts[levelTo].push_back( levelFrom );
			}
			else {
				transitionsOneWay.emplace( actual );
			}
		}

		if ( availablePorts[levelFrom].size() == 1 ) {
			deadEnds.emplace( levelFrom );
		}
	}

	// Manually specify duplicate exits.
	/*for ( auto& p : correctedTransitionsFrom ) {
		availablePorts[p.first.areaFromID].push_back( p.first.areaToID );
	}*/

	// Get rid of any area with no exits to process.
	std::vector<uint32_t> purgeList;
	for ( auto& p : availablePorts ) {
		if ( p.second.empty() ) {
			purgeList.push_back( p.first );
			continue;
		}
	}
	for ( uint32_t i : purgeList ) {
		knownAreas.erase( i );
		availablePorts.erase( i );
	}

	//std::vector<uint32_t> accessibleAreas;   // List of all reachable areas (TODO : save somewhere for graphml gen).
	std::vector<uint32_t> strictAvail;       // "Strictly available" areas. These areas don't have all of their exits assigned and are reachable.
	std::vector<uint32_t> availNotMaster;    // "Non-attached available" areas. These areas don't have all of their exits assigned and are NOT reachable.

	// Mark starting area as accessible (duh).
	m_accessibleAreas.insert( rando_config.startingArea );
	strictAvail.push_back( rando_config.startingArea );
	// List all other zones as non-reachable.
	for ( uint32_t id : knownAreas ) {
		if ( id != rando_config.startingArea ) {
			availNotMaster.push_back( id );
		}
	}

	// Process.
	while ( !strictAvail.empty() || !availNotMaster.empty() ) {
		int levelFrom, levelTo;
		uint32_t levelFromID, levelToID;

		if ( strictAvail.size() == 0 && availNotMaster.size() > 0 ) {
			log_printf( "GENERATION ERROR : Reachable map has no more ports left, but some areas are still unassigned.\n" );
			break;
		}

		// No more unconnected zones, now we just need to connect the remaining available exits.
		if ( availNotMaster.size() == 0 ) {
			levelFrom = rand() % strictAvail.size();
			levelTo = rand() % strictAvail.size();

			// Prevent level from looping onto itself.
			if ( levelFrom == levelTo ) {
				if ( strictAvail.size() == 1 ) {
					log_printf( "WARN : %s will loop onto itself!\n", level_get_name(strictAvail[0]) );
				}
				else {
					if ( levelFrom == 0 ) levelFrom++;
					else levelFrom--;
				}
			}

			levelFromID = strictAvail[levelFrom];
			levelToID = strictAvail[levelTo];
		}
		else {
			levelFrom = rand() % strictAvail.size();
			levelTo = rand() % availNotMaster.size();

			// Favor areas with more than one exit remaining.
			if ( availablePorts[availNotMaster[levelTo]].size() == 1 && availNotMaster.size() > 1 ) {
				if ( levelTo == 0 ) levelTo++;
				else levelTo--;
			}

			// Prevent level from looping onto itself.
			/*if ( levelFrom == levelTo ) {
				if ( levelFrom == 0 ) levelFrom++;
				else levelFrom--;
			}*/

			levelFromID = strictAvail[levelFrom];
			levelToID = availNotMaster[levelTo];

			if ( levelFromID == levelToID ) {
				log_printf( "WARN : %s is going to connect with itself!\n", level_get_name(levelFromID) );
			}
		}

		int fromExit = 0;
		int toEntrance = 0;

		// Form linked transition.
		Transition original( levelFromID, availablePorts[levelFromID][fromExit] );
		Transition redirect( availablePorts[levelToID][toEntrance], levelToID );
		m_transitionsMap[original] = redirect;
		m_transitionsMap[mirror(redirect)] = mirror(original);

		// We've just made this area reachable.
		if ( std::find(strictAvail.begin(), strictAvail.end(), levelToID) == strictAvail.end() && m_accessibleAreas.find(levelToID) == m_accessibleAreas.end() ) {
			strictAvail.push_back( levelToID );
			m_accessibleAreas.insert( levelToID );
			vector_remove( availNotMaster, levelToID );
		}

		// Mark exits as used.
		vector_removeAt( availablePorts[levelFromID], fromExit );
		vector_removeAt( availablePorts[levelToID], toEntrance );

		// Remove filled up areas from available list.
		if ( availablePorts[levelFromID].empty() ) {
			availablePorts.erase( levelFromID );
			vector_remove( strictAvail, levelFromID );
		}
		if ( availablePorts[levelToID].empty() ) {
			availablePorts.erase( levelToID );
			vector_remove( strictAvail, levelToID );
		}
	}

	if ( strictAvail.size() == 1 && availNotMaster.empty() ) {
		log_printf( "GENERATION ERROR : This area wasn't processed correctly : %s\n", level_get_name(strictAvail[0]) );
		//log_printf( "        Level has %d unconnected exits!\n", accessibleAreas[0] );
	}

	if ( !availablePorts.empty() ) {
		log_printf( "WARN : Some exits are still not connected!\n" );
		for ( auto& p : availablePorts ) {
			log_printf( "- %s : %d exits\n", level_get_name(p.first), p.second.size() );
		}

		if ( availablePorts.size() == 1 ) {
			auto& p = *availablePorts.begin();

			Transition original( p.first, availablePorts[p.first][0] );
			Transition redirect( availablePorts[p.first][1], p.first );
			m_transitionsMap[original] = redirect;
			m_transitionsMap[mirror(redirect)] = mirror(original);

			log_printf( "- connected those exits together. The level will loop onto itself.\n" );
		}
	}

	// Add these manually since at least one unrandomized zone is reachable from them.
	m_accessibleAreas.insert( levels_t::MOUNTAIN_SLED_RUN );
	m_accessibleAreas.insert( levels_t::APU_ILLAPU_SHRINE );
	m_accessibleAreas.insert( levels_t::CRASH_SITE );
	m_accessibleAreas.insert( levels_t::TELEPORT );

	// Redirect the auto-travel to BB camp after Altar of Ages cutscene.
	Transition altarRedirect = m_transitionsMap[Transition(levels_t::ALTAR_OF_AGES, levels_t::MYSTERIOUS_TEMPLE)];
	m_transitionsMap[Transition(levels_t::ALTAR_OF_AGES, levels_t::BITTENBINDER_CAMP)] = altarRedirect;

	// Remove water levels.
	// We do it after generation to avoid changing the random seed.
	if ( rando_config.skipWaterLevels ) {
		remove_line_level( levels_t::FLOODED_CAVE, levels_t::BITTENBINDER_CAMP, levels_t::RENEGADE_FORT );
		remove_line_level( levels_t::MYSTERIOUS_TEMPLE, levels_t::BITTENBINDER_CAMP, levels_t::ALTAR_OF_AGES );

		m_accessibleAreas.erase( levels_t::FLOODED_CAVE );
		m_accessibleAreas.erase( levels_t::MYSTERIOUS_TEMPLE );
	}

	// Force skip Jag 2.
	m_transitionsMap[Transition(levels_t::GATES_OF_EL_DORADO, levels_t::RUINS_OF_EL_DORADO)]
		= Transition(levels_t::GATES_OF_EL_DORADO, levels_t::ANCIENT_EL_DORADO);
}



// Legacy rando map generator.
// Although it isn't the same algorithm as the original Dolphin Scripting version, the idea
// stays the same and the maps generated are very similar.
// This method is prone to lots of softlocks.

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

void RandoMap::generateLegacy()
{
	std::vector<Transition> possibleRedirections;

	// List all level transitions that can make it to the final map.
	for ( auto& p : level_infos ) {
		uint32_t levelFrom = p.first;
		Area* area = p.second;
		if ( is_area_excluded(levelFrom) ) continue;

		for ( const Exit& exit : area->exits ) {
			uint32_t levelTo = level_get_by_crc( exit.areaCRC );
			if ( is_area_excluded(levelTo) ) continue;

			Transition t( levelFrom, levelTo );
			possibleRedirections.push_back( t );
		}
	}

	// Pick random transitions from our list to replace original ones.
	for ( auto& p : level_infos ) {
		uint32_t levelFrom = p.first;
		Area* area = p.second;
		if ( is_area_excluded(levelFrom) ) continue;

		for ( const Exit& exit : area->exits ) {
			uint32_t levelTo = level_get_by_crc( exit.areaCRC );
			if ( is_area_excluded(levelTo) ) continue;

			Transition original( levelFrom, levelTo );
			if ( is_transition_disabled(original) ) {
				continue;
			}

			int redirect = get_random_one_way_transition( original, possibleRedirections );
			if ( redirect != -1 ) {
				Transition r( original.areaFromID, possibleRedirections[redirect].areaToID );
				m_transitionsMap.insert( std::pair<Transition, Transition>(original, r) );
				possibleRedirections.erase( possibleRedirections.begin() + redirect );
			}
		}
	}
}

void RandoMap::generateMap()
{
	if ( rando_config.legacy ) {
		generateLegacy();
	}
	else {
		generateLinkedTransitions();
	}
	write_graphml();
}

bool RandoMap::spoofTransition( Transition& o )
{
	// Where the actual hijacking takes place.
	Transition original, redirect;
	original = o;

	// Skip Viracocha cutscene entirely (then proceed with regular hijacking).
	if ( original.areaToID == levels_t::VIRACOCHA_MONOLITHS_CUTSCENE ) {
		original.areaToID = levels_t::VIRACOCHA_MONOLITHS;
	}

	auto it = m_transitionsMap.find( original );
	if ( it == m_transitionsMap.end() ) {
		return false;
	}
	else {
		o = it->second;
		return true;
	}
}



Transition rando_redirect_transition;

static bool can_escape_apu_illapu()
{
	ItemStruct* items = (ItemStruct*) 0x8EEB90;
	EIHarry* harry = *((EIHarry**) 0x917034);

	// Can finish fight?
	if (
		harry->m_breakdance ||
		items[3].m_unlocked ||   // TNT.
		items[2].m_unlocked      // Pickaxes.
	) {
		return true;
	}

	// Super sling.
	if ( harry->m_superSling && items[0].m_unlocked ) {
		return true;
	}

	// Item slide.
	if ( harry->m_heroicDash && (
		items[0].m_unlocked ||   // Sling.
		items[1].m_unlocked ||   // Torch.
		items[3].m_unlocked ||   // TNT.
		items[4].m_unlocked ||   // Shield.
		items[6].m_unlocked ||   // Gas Mask.
		items[7].m_unlocked ||   // Canteen.
		items[8].m_unlocked      // Stink Bombs.
	)) {
		return true;
	}

	return false;
}

void prevent_transition_softlock()
{
	uint32_t currentAreaCRC = *((uint32_t*) 0x917088);
	uint32_t currentAreaID = level_get_by_crc( currentAreaCRC );
	EIHarry* harry = *((EIHarry**) 0x917034);

	if ( currentAreaID == levels_t::APU_ILLAPU_SHRINE ) {
		if ( !can_escape_apu_illapu() ) {
			log_printf( "Missing requirements to complete Apu Illapu Shrine. Kicking you out!\n" );
			harry->m_collisionFilter = 0;
			harry->m_position.x = -24.0F;
			harry->m_position.y =  38.0F;
			harry->m_position.z =  20.0F;
		}
	}
	else {
		Transition t( rando_redirect_transition.areaFromID, currentAreaID );

		if ( softlockableTransitions.find(t) != softlockableTransitions.end() ) {
			log_printf( "Detected softlockable transition!\n" );

			EIHarry* harry = *((EIHarry**) 0x917034);
			harry->m_position.z += 12.0F;
		}
	}
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
