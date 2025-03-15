#include "rando/rando.h"

#include "ptle/levels/level_info.h"

#include <iostream>
#include <fstream>
#include <set>
using namespace std;


#define STARTING_AREA_COLOR "#ff8000"  // Orange
#define UPGRADE_AREAS_COLOR "#0080ff"  // Blue
#define IMPORTANT_STORY_TRIGGER_AREAS_COLOR = "#ff0000"  // Red
#define UNRANDOMIZED_EDGE_COLOR = "#000000"  // Black
#define CLOSED_DOOR_EDGE_COLOR = "#ff0000"  // Red


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


static void write_vertices( ofstream& os )
{
	std::set<uint32_t> randomizedAreas;

	// TODO : ensure that the starting area is in this set.

	for ( auto& p : rando_transitions_map ) {
		randomizedAreas.emplace( p.second.areaFromID );
		randomizedAreas.emplace( p.second.areaToID );
	}

	int px = 0;
	for ( uint32_t levelID : randomizedAreas ) {
		os << "<node positionX=\"" << (px % 10) * 40 << "\" positionY=\"" << (px / 10) * 40 << "\" id=\"" << levelID << "\" mainText=\"" << level_get_name(levelID) << "\" />";
		px += 1;
	}
}

static void write_edges( ofstream& os )
{
	std::set<Transition> processedTransitions;
	std::set<Transition> transitionsTwoWay;
	std::set<Transition> transitionsOneWay;

	int edgeID = 0;

	for ( auto& p : rando_transitions_map ) {
		Transition actual = { p.second.areaFromID, p.second.areaToID };
		Transition reverse = { p.second.areaToID, p.second.areaFromID };

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
	ofstream graph( "rando.graphml" );

	graph << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	graph << "<graphml><graph id=\"Graph\" uidGraph=\"1\" uidEdge=\"1\">";

	write_vertices( graph );
	write_edges( graph );

	graph << "</graph></graphml>";

	graph.close();
}
