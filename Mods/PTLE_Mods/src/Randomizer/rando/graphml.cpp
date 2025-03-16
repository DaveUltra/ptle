#include "rando/rando.h"

#include "ptle/levels/level_info.h"

#include <iostream>
#include <fstream>
#include <set>
using namespace std;


#define STARTING_AREA_COLOR "#ff8000"  // Orange
#define UPGRADE_AREAS_COLOR "#0080ff"  // Blue
#define IMPORTANT_STORY_TRIGGER_AREAS_COLOR "#ff0000"  // Red
#define UNRANDOMIZED_EDGE_COLOR "#000000"  // Black
#define CLOSED_DOOR_EDGE_COLOR "#ff0000"  // Red


static const uint32_t UPGRADE_AREAS[] = {
	levels_t::PLANE_COCKPIT,          // Canteen
	levels_t::BITTENBINDER_CAMP,      // Sling + Rising Strike
	levels_t::MOUTH_OF_INTI,          // Torch
	levels_t::SCORPION_TEMPLE,        // Torch, temporary due to the current Scorpion Temple anti-softlock
	levels_t::NATIVE_VILLAGE,         // Shield
	levels_t::RENEGADE_HEADQUARTERS,  // Gas Mask
	levels_t::CAVERN_LAKE,            // Raft
	levels_t::MOUNTAIN_SLED_RUN,      // Raft
	levels_t::MOUNTAIN_OVERLOOK,      // Pickaxes
	levels_t::APU_ILLAPU_SHRINE,      // TNT
	levels_t::FLOODED_COURTYARD,      // Dash
	levels_t::TURTLE_MONUMENT,        // Dive
};

static const uint32_t IMPORTANT_STORY_TRIGGER_AREAS[] = {
	levels_t::ALTAR_OF_AGES,
	levels_t::ST_CLAIRE_EXCAVATION_CAMP_DAY,
	levels_t::GATES_OF_EL_DORADO,
};

// Don't display these in the graph.
static const uint32_t DONT_SHOW[] = {
	levels_t::ST_CLAIRE_EXCAVATION_CAMP_NIGHT,
	levels_t::RUINS_OF_EL_DORADO,
	levels_t::ANCIENT_EL_DORADO,
	levels_t::SCORPION_SPIRIT,
	levels_t::PENGUIN_SPIRIT,
	levels_t::MONKEY_SPIRIT,
	levels_t::PLANE_CUTSCENE,
	levels_t::VIRACOCHA_MONOLITHS_CUTSCENE,
	levels_t::KABOOM,
	levels_t::PICKAXE_RACE,
	levels_t::WHACK_A_TUCO,
	levels_t::TUCO_SHOOT,
	levels_t::RAFT_BOWLING,
	levels_t::BETA_VOLCANO,
};


static void write_fillStyle( ofstream& os, const char* color )
{
	os << "ownStyles=\"{&quot;0&quot;:{" << "&quot;fillStyle&quot;: &quot;" << color << "&quot;" << "}}\"";
}

static void write_vertices( ofstream& os, const std::set<uint32_t>& accessibleAreas )
{
	// TODO : Tell me with a straight face that the STL isn't stupid.
	const std::set<uint32_t> upgradeAreas( UPGRADE_AREAS, UPGRADE_AREAS+_countof(UPGRADE_AREAS) );
	const std::set<uint32_t> importantStoryAreas( IMPORTANT_STORY_TRIGGER_AREAS, IMPORTANT_STORY_TRIGGER_AREAS+_countof(IMPORTANT_STORY_TRIGGER_AREAS) );

	int px = 0;
	for ( uint32_t levelID : accessibleAreas ) {
		os << "<node positionX=\"" << (px % 10) * 100 << "\" positionY=\"" << (px / 10) * 100 << "\" id=\"" << levelID << "\" mainText=\"" << level_get_name(levelID) << "\" ";

		// Node coloring.
		if ( levelID == rando_config.startingArea )
			write_fillStyle( os, STARTING_AREA_COLOR );
		else if ( upgradeAreas.find(levelID) != upgradeAreas.end() )
			write_fillStyle( os, UPGRADE_AREAS_COLOR );
		else if ( importantStoryAreas.find(levelID) != importantStoryAreas.end() )
			write_fillStyle( os, IMPORTANT_STORY_TRIGGER_AREAS_COLOR );

		os << " />";
		px += 1;
	}
}

static void write_edges( ofstream& os, const std::set<Transition>& possibleTransitions )
{
	std::set<Transition> transitionsTwoWay;
	std::set<Transition> transitionsOneWay;

	int edgeID = 0;

	// Sort by directed / undirected.
	for ( const Transition& actual : possibleTransitions ) {
		Transition reverse = actual.mirror();

		// Promote to two-way.
		if ( transitionsOneWay.find(reverse) != transitionsOneWay.end() ) {
			transitionsOneWay.erase(reverse);
			transitionsTwoWay.emplace( actual );
		}
		else {
			transitionsOneWay.emplace( actual );
		}
	}

	for ( const Transition& t : transitionsTwoWay ) {
		os << "<edge id=\"" << edgeID << "\" source=\"" << t.areaFromID << "\" target=\"" << t.areaToID << "\" isDirect=\"false\" />";
		edgeID++;
	}

	for ( const Transition& t : transitionsOneWay ) {
		os << "<edge id=\"" << edgeID << "\" source=\"" << t.areaFromID << "\" target=\"" << t.areaToID << "\" isDirect=\"true\" />";
		edgeID++;
	}
}

void write_graphml()
{
	const std::set<uint32_t> dontShow( DONT_SHOW, DONT_SHOW+_countof(DONT_SHOW) );

	ofstream graph( "rando.graphml" );

	graph << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	graph << "<graphml><graph id=\"Graph\" uidGraph=\"1\" uidEdge=\"1\">";

	std::set<uint32_t> accessibleAreas;
	std::set<Transition> possibleTransitions;

	for ( auto& p : transition_infos ) {
		for ( const Area& area : p.second ) {
			uint32_t fromID = level_get_by_crc(area.areaID);
			if ( dontShow.find(fromID) != dontShow.end() ) continue;

			for ( const Exit& exit : area.exits ) {
				uint32_t toID = level_get_by_crc(exit.areaID);
				if ( dontShow.find(toID) != dontShow.end() ) continue;

				Transition original( fromID, toID );
				Transition redirect = original;

				spoof_transition( redirect );

				possibleTransitions.emplace( Transition(original.areaFromID, redirect.areaToID) );
				accessibleAreas.emplace( original.areaFromID );
				accessibleAreas.emplace( redirect.areaToID );
			}
		}
	}

	write_vertices( graph, accessibleAreas );
	write_edges( graph, possibleTransitions );

	graph << "</graph></graphml>";

	graph.close();
}
