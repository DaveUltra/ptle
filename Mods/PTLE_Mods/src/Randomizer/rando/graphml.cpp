#include "rando/rando.h"

#include "item_rando.h"

#include "gizmod/Gizmod.h"
#include "gizmod/GizmodPlugin.h"

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
	levelCRC::BITTENBINDER_CAMP,      // Rising Strike
	levelCRC::FLOODED_COURTYARD,      // Dash
	levelCRC::TURTLE_MONUMENT,        // Dive
};

// These are the zones required to reach the final boss.
static const uint32_t IMPORTANT_STORY_TRIGGER_AREAS[] = {
	levelCRC::ALTAR_OF_AGES,
	levelCRC::ST_CLAIRE_EXCAVATION_CAMP_DAY,
	levelCRC::GATES_OF_EL_DORADO,
};

// Don't display these in the graph.
static const uint32_t DONT_SHOW[] = {
	levelCRC::ST_CLAIRE_EXCAVATION_CAMP_NIGHT,
	levelCRC::RUINS_OF_EL_DORADO,
	levelCRC::ANCIENT_EL_DORADO,
	levelCRC::SCORPION_SPIRIT,
	levelCRC::PENGUIN_SPIRIT,
	levelCRC::MONKEY_SPIRIT,
	levelCRC::PLANE_CUTSCENE,
	levelCRC::VIRACOCHA_MONOLITHS_CUTSCENE,
	levelCRC::KABOOM,
	levelCRC::PICKAXE_RACE,
	levelCRC::WHACK_A_TUCO,
	levelCRC::TUCO_SHOOT,
	levelCRC::RAFT_BOWLING,
	levelCRC::BETA_VOLCANO,
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
	std::set<uint32_t> upgradeAreas;
	const std::set<uint32_t> importantStoryAreas( IMPORTANT_STORY_TRIGGER_AREAS, IMPORTANT_STORY_TRIGGER_AREAS+_countof(IMPORTANT_STORY_TRIGGER_AREAS) );

	uint32_t itemLocations[8];
	get_item_locations( itemLocations );
	upgradeAreas.insert( itemLocations, itemLocations+_countof(itemLocations) );

	upgradeAreas.insert( UPGRADE_AREAS, UPGRADE_AREAS+_countof(UPGRADE_AREAS) );

	int px = 0;
	for ( uint32_t levelID : accessibleAreas ) {
		// Remove parentheses from names (since we removed their counterpart anyway).
		std::string name = level_get_name(level_get_by_crc(levelID));
		if ( levelID != levelCRC::TWIN_OUTPOSTS_UNDERWATER ) {
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

#include "utils/log.h"
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
		Transition aToB( p.second.areaFromID, p.first.areaToID );
		Transition bToA( p.second.areaToID, p.first.areaFromID );

		os << "<edge id=\"" << edgeID << "\" source=\"" << t.areaFromID << "\" target=\"" << t.areaToID << "\" isDirect=\"false\" ";

		bool lockedDoor = (softlockableTransitions.find(aToB) != softlockableTransitions.end()) || (softlockableTransitions.find(bToA) != softlockableTransitions.end());
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

	std::wstring cfgDir = Gizmod::getThisPlugin()->getConfigDirectory();
	ofstream graph( cfgDir + L"/rando.graphml" );

	graph << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	graph << "<graphml><graph id=\"Graph\" uidGraph=\"1\" uidEdge=\"1\">";

	std::map<Transition, Transition> possibleTransitions;

	for ( auto& p : level_infos ) {
		Area* area = p.second;

		if ( dontShow.find(p.first) != dontShow.end() ) continue;
		if ( rando_map.getAccessibleAreas().find(p.first) == rando_map.getAccessibleAreas().end() ) continue;

		for ( const Exit& exit : area->exits ) {
			if ( dontShow.find(exit.areaCRC) != dontShow.end() ) continue;

			Transition original( p.first, exit.areaCRC );
			Transition redirect = original;
			rando_map.spoofTransition( redirect );

			if ( rando_map.getAccessibleAreas().find(redirect.areaToID) == rando_map.getAccessibleAreas().end() ) continue;

			possibleTransitions.emplace( original, redirect );
		}
	}

	write_vertices( graph, rando_map.getAccessibleAreas() );
	write_edges( graph, possibleTransitions );

	graph << "</graph></graphml>";

	graph.close();
}
