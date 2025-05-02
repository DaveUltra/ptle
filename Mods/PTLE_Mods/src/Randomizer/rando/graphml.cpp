#include "rando/rando.h"

#include "ptle/levels/level_info.h"

#include <iostream>
#include <fstream>
#include <string>
#include <set>
using namespace std;


#define STARTING_AREA_COLOR "#ff8000"  // Orange
#define UPGRADE_AREAS_COLOR "#0080ff"  // Blue
#define IMPORTANT_STORY_TRIGGER_AREAS_COLOR "#ff0000"  // Red
#define UNRANDOMIZED_EDGE_COLOR "#707070"  // Gray
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

static void write_edgeStyle( ofstream& os, const char* color, bool dashed )
{
	int i = 0;
	os << "ownStyles=\"{&quot;0&quot;:{";
		if ( color ) { os << "&quot;strokeStyle&quot;: &quot;" << color << "&quot;"; i++; }
		if ( dashed ) { if (i!=0) os << ','; os << "&quot;lineDash&quot;: 2"; i++; }
	os << "}}\"";
}

static void write_vertices( ofstream& os, const std::set<uint32_t>& accessibleAreas )
{
	// TODO : Tell me with a straight face that the STL isn't stupid.
	const std::set<uint32_t> upgradeAreas( UPGRADE_AREAS, UPGRADE_AREAS+_countof(UPGRADE_AREAS) );
	const std::set<uint32_t> importantStoryAreas( IMPORTANT_STORY_TRIGGER_AREAS, IMPORTANT_STORY_TRIGGER_AREAS+_countof(IMPORTANT_STORY_TRIGGER_AREAS) );

	int px = 0;
	for ( uint32_t levelID : accessibleAreas ) {
		// Remove parentheses from names (since we removed their counterpart anyway).
		std::string name = level_get_name(levelID);
		if ( levelID != levels_t::TWIN_OUTPOSTS_UNDERWATER ) {
			size_t paren = name.find_last_of('(');
			if ( paren != std::string::npos ) {
				name = name.substr(0, paren - 1);
			}
		}
		os << "<node positionX=\"" << (px % 10) * 100 << "\" positionY=\"" << (px / 10) * 100 << "\" id=\"" << levelID << "\" mainText=\"" << name << "\" ";

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

static void write_edges( ofstream& os, const std::map<Transition, Transition>& possibleTransitions )
{
	std::map<Transition, Transition> transitionsTwoWay;
	std::map<Transition, Transition> transitionsOneWay;

	int edgeID = 0;

	// Sort by directed / undirected.
	for ( const auto& p : possibleTransitions ) {
		Transition real( p.first.areaFromID, p.second.areaToID );
		Transition incoming( p.second.areaFromID, p.first.areaToID );

		// Promote to two-way.
		if ( transitionsOneWay.find(real.mirror()) != transitionsOneWay.end() ) {
			transitionsOneWay.erase(real.mirror());
			transitionsTwoWay.emplace( real, incoming );
		}
		else {
			transitionsOneWay.emplace( real, incoming );
		}
	}

	for ( const auto& p : transitionsTwoWay ) {
		const Transition& t = p.first;
		Transition orig( p.first.areaFromID, p.second.areaToID );

		os << "<edge id=\"" << edgeID << "\" source=\"" << t.areaFromID << "\" target=\"" << t.areaToID << "\" isDirect=\"false\" ";

		bool lockedDoor = (softlockableTransitions.find(orig.mirror()) != softlockableTransitions.end());
		if ( lockedDoor ) {
			write_edgeStyle( os, CLOSED_DOOR_EDGE_COLOR, false );
		}

		os << " />";
		edgeID++;
	}

	for ( const auto& p : transitionsOneWay ) {
		const Transition& t = p.first;

		os << "<edge id=\"" << edgeID << "\" source=\"" << t.areaFromID << "\" target=\"" << t.areaToID << "\" isDirect=\"true\" ";

		bool lockedDoor = (softlockableTransitions.find(t) != softlockableTransitions.end());
		if ( lockedDoor ) {
			write_edgeStyle( os, CLOSED_DOOR_EDGE_COLOR, true );
		}
		else {
			write_edgeStyle( os, 0, true );
		}

		os << " />";
		edgeID++;
	}
}

void write_graphml()
{
	std::set<uint32_t> dontShow( DONT_SHOW, DONT_SHOW+_countof(DONT_SHOW) );

	ofstream graph( "rando.graphml" );

	graph << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	graph << "<graphml><graph id=\"Graph\" uidGraph=\"1\" uidEdge=\"1\">";

	std::map<Transition, Transition> possibleTransitions;

	for ( auto& p : level_infos ) {
		uint32_t fromID = p.first;
		Area* area = p.second;
		if ( dontShow.find(fromID) != dontShow.end() ) continue;
		if ( rando_map.getAccessibleAreas().find(fromID) == rando_map.getAccessibleAreas().end() ) continue;

		for ( const Exit& exit : area->exits ) {
			uint32_t toID = level_get_by_crc(exit.areaCRC);
			if ( dontShow.find(toID) != dontShow.end() ) continue;

			Transition original( fromID, toID );
			Transition redirect = original;

			rando_map.spoofTransition( redirect );

			possibleTransitions.emplace( original, redirect );
		}
	}

	write_vertices( graph, rando_map.getAccessibleAreas() );
	write_edges( graph, possibleTransitions );

	graph << "</graph></graphml>";

	graph.close();
}
