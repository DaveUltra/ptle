#include "rando/rando.h"

#include "ptle/levels/level_info.h"

#include "PitfallPlugin.h"

#include "ptle/EIHarry.h"

#include "utils/log.h"

#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>


enum TwinOutpostsWaterExits
{
	JUNGLE_TO_WATER  = 0x9D1A6D4A,
	BURNING_TO_WATER = 0x7C65128A,
	WATER_TO_JUNGLE  = 0x00D15464,
	WATER_TO_BURNING = 0xE1AE2BA4
};


RandoMap rando_map;


std::set<Transition> disabledTransitions;

// These transitions can only be traversed in one direction.
static const Transition ONE_WAY_TRANSITIONS[] = {
	// White Valley geyser.
	Transition( levelCRC::WHITE_VALLEY,      levelCRC::MOUNTAIN_SLED_RUN ),
	// Apu Illapu Shrine geyser.
	Transition( levelCRC::APU_ILLAPU_SHRINE, levelCRC::WHITE_VALLEY ),
	// Apu Illapu Shrine one-way door.
	Transition( levelCRC::MOUNTAIN_SLED_RUN, levelCRC::APU_ILLAPU_SHRINE ),
	// Jungle Canyon waterfall.
	Transition( levelCRC::CAVERN_LAKE,       levelCRC::JUNGLE_CANYON ),
	// After Altar of Ages cutscene.
	Transition( levelCRC::ALTAR_OF_AGES,     levelCRC::BITTENBINDER_CAMP ),
};

// These transitions can lead to softlocks due to closed doors / obstacles.
// Harry will run indefinitely against them and the game will never give us control back.
static const Transition SOFTLOCKABLE_TRANSITIONS[] = {
	Transition( levelCRC::ST_CLAIRE_EXCAVATION_CAMP_DAY, levelCRC::FLOODED_COURTYARD ),   // St.Claire wall (requires TNT).
	Transition( levelCRC::TWIN_OUTPOSTS,                 levelCRC::FLOODED_COURTYARD ),   // Monkey door.
	Transition( levelCRC::SCORPION_TEMPLE,               levelCRC::EYES_OF_DOOM ),        // Door to scorp temple.
	Transition( levelCRC::MOUNTAIN_OVERLOOK,             levelCRC::EYES_OF_DOOM ),        // Scorpion door.
	Transition( levelCRC::EKKEKO_ICE_CAVERN,             levelCRC::VALLEY_OF_SPIRITS ),   // Ice wall (requires TNT).
	Transition( levelCRC::MOUNTAIN_SLED_RUN,             levelCRC::COPACANTI_LAKE ),      // Three crystals door.
	Transition( levelCRC::COPACANTI_LAKE,                levelCRC::VALLEY_OF_SPIRITS ),   // Spirit door.
	Transition( levelCRC::BATTERED_BRIDGE,               levelCRC::ALTAR_OF_HUITACA ),    // Spider web (not a softlock, but it is a locked way).
};
const std::set<Transition> softlockableTransitions( SOFTLOCKABLE_TRANSITIONS, SOFTLOCKABLE_TRANSITIONS+_countof(SOFTLOCKABLE_TRANSITIONS) );


std::map<Transition, uint32_t> correctedTransitionsFrom;

// Putting levels in this list will make the randomizer ignore them as starting areas
// and not randomize them, if their transitions haven't been excluded yet.
std::set<uint32_t> excludedLevels;

std::vector<uint32_t> startingAreas;


bool completed_monkey_temple   = false;
bool completed_scorpion_temple = false;
bool completed_penguin_temple  = false;



static inline void disableTransition( levelCRC::Enum areaFromCRC, levelCRC::Enum areaToCRC )
{
	Transition t( areaFromCRC, areaToCRC );
	disabledTransitions.emplace( t );
}
static inline void disableTransition2( levelCRC::Enum areaFromCRC, levelCRC::Enum areaToCRC )
{
	Transition t( areaFromCRC, areaToCRC );
	disabledTransitions.emplace( t );
	disabledTransitions.emplace( t.mirror() );
}

// TODO : Misnamed. These are the levels we don't want to randomize.
static void set_excluded_levels()
{
	static const uint32_t excluded[] = {
		// Ignore crash site for now, as this causes losing all acquired upgrades & items.
		levelCRC::CRASH_SITE,

		// Ignore native games.
		levelCRC::KABOOM,
		levelCRC::PICKAXE_RACE,
		levelCRC::RAFT_BOWLING,
		levelCRC::TUCO_SHOOT,
		levelCRC::WHACK_A_TUCO,

		// Avoid warping to spirit fights without warning.
		levelCRC::MONKEY_SPIRIT,
		levelCRC::SCORPION_SPIRIT,
		levelCRC::PENGUIN_SPIRIT,

		// Skip viracocha & plane crash cutscenes.
		levelCRC::PLANE_CUTSCENE,
		levelCRC::VIRACOCHA_MONOLITHS_CUTSCENE,

		// Don't randomize teleporter entrances.
		levelCRC::TELEPORT,

		// Don't randomize this hell hole.
		levelCRC::TWIN_OUTPOSTS_UNDERWATER,

		// Don't warp straight to bosses.
		levelCRC::RUINS_OF_EL_DORADO,
		levelCRC::ANCIENT_EL_DORADO,
	};

	excludedLevels.insert( excluded, excluded+_countof(excluded) );
}

static void set_disabled_transitions()
{
	// The 3 Spirit Fights are not randomized,
	// because that causes issues with the transformation cutscene trigger.
	// Plus it wouldn't really improve anything, given that the Temples are randomized anyway.
	disableTransition2( levelCRC::MONKEY_TEMPLE, levelCRC::MONKEY_SPIRIT );
	disableTransition2( levelCRC::SCORPION_TEMPLE, levelCRC::SCORPION_SPIRIT );
	disableTransition2( levelCRC::PENGUIN_TEMPLE, levelCRC::PENGUIN_SPIRIT );
	// The 5 Native Games are currently chosen to not be randomized.
	// If we at some point decide to randomize them anyway we'll have to do some rigorous testing,
	// because it's very much possible this will cause some bugs.
	disableTransition2( levelCRC::NATIVE_VILLAGE, levelCRC::WHACK_A_TUCO );
	disableTransition2( levelCRC::NATIVE_VILLAGE, levelCRC::TUCO_SHOOT );
	disableTransition2( levelCRC::NATIVE_VILLAGE, levelCRC::RAFT_BOWLING );
	disableTransition2( levelCRC::NATIVE_VILLAGE, levelCRC::PICKAXE_RACE );
	disableTransition2( levelCRC::NATIVE_VILLAGE, levelCRC::KABOOM );
	// The Endgame transitions are not randomized.
	// Currently there are no plans to randomize these transitions.
	disableTransition2( levelCRC::ST_CLAIRE_EXCAVATION_CAMP_DAY, levelCRC::ST_CLAIRE_EXCAVATION_CAMP_NIGHT );
	disableTransition( levelCRC::GATES_OF_EL_DORADO, levelCRC::RUINS_OF_EL_DORADO );
	disableTransition( levelCRC::RUINS_OF_EL_DORADO, levelCRC::ANCIENT_EL_DORADO );
	disableTransition( levelCRC::ANCIENT_EL_DORADO, levelCRC::GATES_OF_EL_DORADO );
	// The Unused Beta Volcano Level is not randomized yet,
	// but this can absolutely be randomized later at some point.
	// It will require some special code though.
	disableTransition( levelCRC::BETA_VOLCANO, levelCRC::JUNGLE_CANYON );
	disableTransition( levelCRC::BETA_VOLCANO, levelCRC::PLANE_COCKPIT );
}

static void set_starting_areas()
{
	for ( const auto& p : level_infos ) {
		switch ( p.first )
		{
		case levelCRC::APU_ILLAPU_SHRINE:                // You'll get kicked out by softlock prevention anyway.

		// Gives too much progression.
		case levelCRC::ST_CLAIRE_EXCAVATION_CAMP_DAY:    // Gives TNT.
		case levelCRC::ST_CLAIRE_EXCAVATION_CAMP_NIGHT:  // Gives all items + access to El Dorado.
		case levelCRC::RUINS_OF_EL_DORADO:               // Supai fight.
		case levelCRC::ANCIENT_EL_DORADO:                // Pusca fight.

		// Temples.
		case levelCRC::MONKEY_TEMPLE: case levelCRC::MONKEY_SPIRIT:
		case levelCRC::SCORPION_TEMPLE: case levelCRC::SCORPION_SPIRIT:
		case levelCRC::PENGUIN_TEMPLE: case levelCRC::PENGUIN_SPIRIT:

		// Native Games.
		case levelCRC::KABOOM:
		case levelCRC::PICKAXE_RACE:
		case levelCRC::RAFT_BOWLING:
		case levelCRC::TUCO_SHOOT:
		case levelCRC::WHACK_A_TUCO:

		// Cutscenes.
		case levelCRC::PLANE_CUTSCENE:
		case levelCRC::VIRACOCHA_MONOLITHS_CUTSCENE:

		// Beta levels (unrandomized for now).
		case levelCRC::BETA_VOLCANO:
			break;

		default:
			startingAreas.push_back( p.first );
			break;
		}
	}
}

static void set_corrected_transitions()
{
	// Dark cave connecting Jungle Trail to Flooded Courtyard.
	//correctedTransitionsFrom.emplace( Transition(levels_t::FLOODED_COURTYARD, levels_t::JUNGLE_TRAIL), 0x402D3708 );
	//correctedTransitionsFrom.emplace( Transition(levels_t::JUNGLE_TRAIL, levels_t::FLOODED_COURTYARD), 0x1AAF2535 );
	correctedTransitionsFrom.emplace( Transition(0x5CC8D550, levelCRC::JUNGLE_TRAIL), 0x402D3708 );
	correctedTransitionsFrom.emplace( Transition(0x5CC8D550, levelCRC::FLOODED_COURTYARD), 0x1AAF2535 ); /*0x5F29C550*/
}

bool is_area_excluded( uint32_t levelID )
{
	return excludedLevels.find( levelID ) != excludedLevels.end();
}

bool is_transition_disabled( const Transition& t )
{
	return disabledTransitions.find( t ) != disabledTransitions.end();
}



// in transition_infos.cpp.
bool load_transition_infos();

// in idols_infos.cpp.
bool load_idols_infos();



void rando_init()
{
	if ( !load_transition_infos() ) {
		return;
	}
	if ( !load_idols_infos() ) {
		return;
	}

	set_excluded_levels();
	set_disabled_transitions();
	set_starting_areas();
	set_corrected_transitions();

	// Choose random seed and starting area by default.
	rando_config.seed = time(0);

	// Config.
	load_config();

	// Set seed and starting area if unspecified by the config.
	srand( rando_config.seed );
	if ( rando_config.startingArea == 0 ) {
		rando_config.startingArea = startingAreas[rand() % startingAreas.size()];
	}

	display_config();
}



//
// Helpers & shortcut functions.
//

static inline Transition mirror( const Transition& t )
{
	return Transition( t.areaToID, t.areaFromID );
}

template<typename T>
static inline void vector_removeAt( std::vector<T>& v, int p )
{
	v.erase( v.begin() + p );
}

template<typename T>
static inline void vector_remove( std::vector<T>& v, const T& val )
{
	auto it = std::remove(v.begin(), v.end(), val);
	if ( it != v.end() )
		v.erase( it );
}



static uint32_t find_original_exit_to( uint32_t cur, uint32_t dst )
{
	for ( const Exit& exit : level_infos[cur]->exits ) {
		auto it = rando_map.getTransitionsMap().find( Transition(cur, exit.areaCRC) );
		if ( it != rando_map.getTransitionsMap().end() && it->second.areaToID == dst ) {
			return exit.areaCRC;
		}
	}
	return dst;
}

// Removes a level with two exits ("line" level) by connecting its randomized destinations to each other.
static void remove_line_level( uint32_t level, uint32_t exitA, uint32_t exitB )
{
	auto& map = rando_map.getTransitionsMap();

	// Get which zones have been mapped to the exits of the level we want to nuke.
	uint32_t levelA = exitA, levelB = exitB;

	auto itA = map.find( Transition(level, exitA) );
	if ( itA != map.end() ) {
		levelA = itA->second.areaToID;
	}
	auto itB = map.find( Transition(level, exitB) );
	if ( itB != map.end() ) {
		levelB = itB->second.areaToID;
	}

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
			uint32_t levelTo = exit.areaCRC;

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

	// Get rid of any area with no exits to process.
	std::vector<uint32_t> purgeList;
	for ( auto& p : availablePorts ) {
		if ( p.second.empty() ) {
			purgeList.push_back( p.first );
			continue;
		}
	}
	log_printf( " - %d levels purged (no exits)\n", purgeList.size() );
	for ( uint32_t i : purgeList ) {
		knownAreas.erase( i );
		availablePorts.erase( i );
	}

	std::vector<uint32_t> strictAvail;       // "Strictly available" areas. These areas don't have all of their exits assigned and are reachable.
	std::vector<uint32_t> availNotMaster;    // "Non-attached available" areas. These areas don't have all of their exits assigned and are NOT reachable.

	// Mark starting area as accessible (duh).
	m_accessibleAreas.insert( rando_config.startingArea );
	strictAvail.push_back( rando_config.startingArea );

	// List all other zones as non-reachable.
	log_printf( " - %d known areas.\n", knownAreas.size() );
	knownAreas.erase( rando_config.startingArea );
	availNotMaster.insert( availNotMaster.begin(), knownAreas.begin(), knownAreas.end() );

	// Process.
	while ( !strictAvail.empty() || !availNotMaster.empty() ) {
		int levelFrom, levelTo;
		uint32_t levelFromCRC, levelToCRC;

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
					log_printf( "WARN : %s will loop onto itself!\n", level_get_name(level_get_by_crc(strictAvail[0])) );
				}
				else {
					if ( levelFrom == 0 ) levelFrom++;
					else levelFrom--;
				}
			}

			levelFromCRC = strictAvail[levelFrom];
			levelToCRC = strictAvail[levelTo];
		}
		else {
			levelFrom = rand() % strictAvail.size();
			levelTo = rand() % availNotMaster.size();

			// Favor remaining areas with more than one exit.
			if ( availNotMaster.size() > 1 ) {
				bool onlyDeadEnds = true;
				for ( int i = 0; i < availNotMaster.size(); i++ ) {
					if ( availablePorts[availNotMaster[i]].size() != 1 ) {
						onlyDeadEnds = false;
						levelTo = i;
						break;
					}
				}
			}

			// Prevent level from looping onto itself.
			if ( strictAvail[levelFrom] == availNotMaster[levelTo] ) {
				if ( levelFrom == 0 ) levelFrom++;
				else levelFrom--;
			}

			levelFromCRC = strictAvail[levelFrom];
			levelToCRC = availNotMaster[levelTo];

			if ( levelFromCRC == levelToCRC ) {
				log_printf( "WARN : %s is going to connect with itself!\n", level_get_name(level_get_by_crc(levelToCRC)) );
			}
		}

		int fromExit = 0;
		int toEntrance = 0;
		if ( levelFromCRC == levelToCRC ) { // TODO : Temporary, levels shouldn't loop onto themselves.
			toEntrance = 1;
		}

		//log_printf( " - %s (0x%X)   -->   %s (0x%X)\n", level_get_name(level_get_by_crc(levelFromCRC)), levelFromCRC, level_get_name(level_get_by_crc(levelToCRC)), levelToCRC );

		// Form linked transition.
		Transition original( levelFromCRC, availablePorts[levelFromCRC][fromExit] );
		Transition redirect( availablePorts[levelToCRC][toEntrance], levelToCRC );
		m_transitionsMap[original] = redirect;
		m_transitionsMap[mirror(redirect)] = mirror(original);

		// We've just made this area reachable.
		if ( std::find(strictAvail.begin(), strictAvail.end(), levelToCRC) == strictAvail.end() && m_accessibleAreas.find(levelToCRC) == m_accessibleAreas.end() ) {
			strictAvail.push_back( levelToCRC );
			m_accessibleAreas.insert( levelToCRC );
			vector_remove( availNotMaster, levelToCRC );
		}

		// Mark exits as used.
		if ( levelFromCRC == levelToCRC ) { // TODO : Temporary, levels shouldn't loop onto themselves.
			vector_removeAt( availablePorts[levelFromCRC], 0 );
			vector_removeAt( availablePorts[levelFromCRC], 0 );
		} else {
			vector_removeAt( availablePorts[levelFromCRC], fromExit );
			vector_removeAt( availablePorts[levelToCRC], toEntrance );
		}

		// Remove filled up areas from available list.
		if ( availablePorts[levelFromCRC].empty() ) {
			availablePorts.erase( levelFromCRC );
			vector_remove( strictAvail, levelFromCRC );
		}
		if ( availablePorts[levelToCRC].empty() ) {
			availablePorts.erase( levelToCRC );
			vector_remove( strictAvail, levelToCRC );
		}
	}

	log_printf( " - Main process done.\n" );

	if ( strictAvail.size() == 1 && availNotMaster.empty() ) {
		log_printf( "GENERATION ERROR : This area wasn't processed correctly : %s\n", level_get_name(level_get_by_crc(strictAvail[0])) );
		//log_printf( "        Level has %d unconnected exits!\n", accessibleAreas[0] );
	}

	if ( !availablePorts.empty() ) {
		log_printf( "GENERATION ERROR : Some exits are still not connected!\n" );
		for ( auto& p : availablePorts ) {
			log_printf( " - %s : %d exits\n", level_get_name(level_get_by_crc(p.first)), p.second.size() );
		}
		//return;
	}


	// Add these manually since at least one unrandomized zone is reachable from them.
	m_accessibleAreas.insert( levelCRC::MOUNTAIN_SLED_RUN );
	m_accessibleAreas.insert( levelCRC::APU_ILLAPU_SHRINE );
	m_accessibleAreas.insert( levelCRC::CRASH_SITE );
	m_accessibleAreas.insert( levelCRC::TELEPORT );

	// Remove water levels.
	// We do it after generation to avoid changing the random seed.
	if ( rando_config.skipWaterLevels ) {
		log_printf( " - Skipping water levels...\n" );

		remove_line_level( levelCRC::FLOODED_CAVE,      levelCRC::BITTENBINDER_CAMP, levelCRC::RENEGADE_FORT );
		remove_line_level( levelCRC::MYSTERIOUS_TEMPLE, levelCRC::BITTENBINDER_CAMP, levelCRC::ALTAR_OF_AGES );

		m_accessibleAreas.erase( levelCRC::FLOODED_CAVE );
		m_accessibleAreas.erase( levelCRC::MYSTERIOUS_TEMPLE );
	}

	// Skip Jag 2 : Warp straight to the Pusca fight.
	if ( rando_config.skipJaguar2 ) {
		const Transition puscaRedirect = Transition(levelCRC::GATES_OF_EL_DORADO, levelCRC::ANCIENT_EL_DORADO);
		m_transitionsMap[Transition(levelCRC::GATES_OF_EL_DORADO, levelCRC::RUINS_OF_EL_DORADO)] = puscaRedirect;
	}

	log_printf( " - Generation ended.\n" );
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
	// Skip Viracocha cutscene entirely (then proceed with regular hijacking).
	if ( o.areaToID == levelCRC::VIRACOCHA_MONOLITHS_CUTSCENE ) {
		o.areaToID = levelCRC::VIRACOCHA_MONOLITHS;
		goto _hijack;
	}

	// Remove twin outpost underwater cave here.
	if ( rando_config.skipWaterLevels ) {
		// Jungle Outpost to Water -> Reroute to Burning Outpost.
		if ( o.areaFromID == TwinOutpostsWaterExits::JUNGLE_TO_WATER ) {
			o.areaFromID = TwinOutpostsWaterExits::WATER_TO_BURNING;
			o.areaToID   = levelCRC::TWIN_OUTPOSTS;
			return true;
		}
		// Burning Outpost to Water -> Reroute to Jungle Outpost.
		else if ( o.areaFromID == TwinOutpostsWaterExits::BURNING_TO_WATER ) {
			o.areaFromID = TwinOutpostsWaterExits::WATER_TO_JUNGLE;
			o.areaToID   = levelCRC::TWIN_OUTPOSTS;
			return true;
		}
	}

	// Refer to transitions map.
_hijack:
	auto it = m_transitionsMap.find( o );
	if ( it == m_transitionsMap.end() ) {
		return false;
	}
	else {
		o = it->second;

		// Immediate spirit fights (avoid warping to spirit fight if completed already).
		if ( rando_config.immediateSpiritFights ) {
			if ( !completed_monkey_temple && o.areaToID == levelCRC::MONKEY_TEMPLE && o.areaFromID == levelCRC::FLOODED_COURTYARD ) {
				o.areaToID   = levelCRC::MONKEY_SPIRIT;
				o.areaFromID = levelCRC::MONKEY_TEMPLE;
			}
			else if ( !completed_scorpion_temple && o.areaToID == levelCRC::SCORPION_TEMPLE && o.areaFromID == levelCRC::EYES_OF_DOOM ) {
				o.areaToID   = levelCRC::SCORPION_SPIRIT;
				o.areaFromID = levelCRC::SCORPION_TEMPLE;
			}
			else if ( !completed_penguin_temple && o.areaToID == levelCRC::PENGUIN_TEMPLE && o.areaFromID == levelCRC::VIRACOCHA_MONOLITHS ) {
				o.areaToID   = levelCRC::PENGUIN_SPIRIT;
				o.areaFromID = levelCRC::PENGUIN_TEMPLE;
			}
		}

		return true;
	}
}



Transition rando_redirect_transition;

static bool can_escape_apu_illapu()
{
	ItemStruct* items = (ItemStruct*) 0x8EEB90;
	EIHarry* harry = PitfallPlugin::getHarry();

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
		items[7].m_unlocked   // Canteen.
		//items[8].m_unlocked      // Stink Bombs.
	)) {
		return true;
	}

	return false;
}

GET_METHOD( 0x4B55F0, void, EIHarry_Teleport, EIHarry*, Matrix4f*, bool, float );
void prevent_transition_softlock()
{
	uint32_t currentAreaCRC = PitfallPlugin::getCurrentLevelCRC();
	EIHarry* harry = PitfallPlugin::getHarry();

	if ( currentAreaCRC == levelCRC::APU_ILLAPU_SHRINE ) {
		if ( !can_escape_apu_illapu() ) {
			log_printf( "Missing requirements to complete Apu Illapu Shrine. Kicking you out!\n" );

			Matrix4f m = harry->m_transformMatrix;
			m.data[3][0] = -24.0F;
			m.data[3][1] =  38.0F;
			m.data[3][2] =  20.0F;
			EIHarry_Teleport( harry, &m, true, 1.0F );
		}
		else {
			log_printf( "Player can escape Apu Illapu on their own.\n" );
		}
	}
	else {
		Transition t( rando_redirect_transition.areaFromID, currentAreaCRC );

		if ( softlockableTransitions.find(t) != softlockableTransitions.end() ) {
			log_printf( "Detected softlockable transition!\n" );

			Matrix4f m = harry->m_transformMatrix;
			m.data[3][2] += 14.0F;
			EIHarry_Teleport( harry, &m, true, 1.0F );
		}
	}
}
