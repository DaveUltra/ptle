#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include <stdint.h>

#include "rando/config/config.h"



// Level exit, used to build transitions.
struct Exit
{
	uint32_t areaCRC;
	uint32_t prevCRC;
	std::string areaName;
	std::vector<std::vector<std::string>> requires;

	Exit( uint32_t prevCRC, uint32_t areaCRC, const std::string& areaName )
		: areaCRC( areaCRC ), prevCRC( prevCRC ), areaName( areaName )
	{
	}
};

// Single level.
struct Area
{
	uint32_t areaCRC;
	std::string name;
	uint32_t defaultEntrance;
	std::vector<Exit> exits;

	Area( uint32_t crc, const std::string& name, uint32_t defaultEntrance )
		: areaCRC( crc ), name( name ), defaultEntrance( defaultEntrance )
	{
	}
};

// Major areas (jungle, mountains, caverns, ...).
typedef std::map<std::string, std::vector<Area*>> MajorAreas;

// One-way level transition.
struct Transition
{
	uint32_t areaFromID;
	uint32_t areaToID;

	Transition()
	{
	}

	Transition( uint32_t from, uint32_t to )
		: areaFromID( from ), areaToID( to )
	{
	}

	inline Transition mirror() const
	{
		return Transition( areaToID, areaFromID );
	}
};

template<>
struct std::hash<Transition>
{
	size_t operator()(const Transition& t) {
		return t.areaFromID | (t.areaToID << 16);
	}
};

inline bool operator<( const Transition& t0, const Transition& t1 )
{
	size_t h0 = std::hash<Transition>()( t0 );
	size_t h1 = std::hash<Transition>()( t1 );
	return h0 < h1;
}

inline bool operator==( const Transition& t0, const Transition& t1 )
{
	return t0.areaFromID == t1.areaFromID && t0.areaToID == t1.areaToID;
}


// Idol locations.
struct Idol
{
	uint32_t m_levelCRC;
	uint32_t m_uniqueID;
	bool m_duplicate;
};

typedef std::map<uint32_t, std::vector<Idol>> IdolsList;



// Map.
class RandoMap
{
public:

	void generateMap();

	bool spoofTransition( Transition& o );

	inline const std::set<uint32_t>& getAccessibleAreas() const { return m_accessibleAreas; }
	inline std::map<Transition, Transition>& getTransitionsMap() { return m_transitionsMap; }


private:

	void generateLegacy();
	void generateLinkedTransitions();

	std::set<uint32_t> m_accessibleAreas;
	std::map<Transition, Transition> m_transitionsMap;   // Exit (currentLevel, normalDest) -> Entrance (whereFrom, redirectedDest).
};

extern RandoMap rando_map;



// Levels & transitions, loaded from "transition_infos.json".
extern MajorAreas transition_infos;

// Idols locations.
extern IdolsList idols_info;

// Area info for each level ID.
extern std::map<uint32_t, Area*> level_infos;

extern const std::set<Transition> softlockableTransitions;

extern bool completed_monkey_temple;
extern bool completed_scorpion_temple;
extern bool completed_penguin_temple;



// Load configuration.
void rando_init();

void randomize_shaman_shop();
void patch_shaman_shop();

void prevent_transition_softlock();

void write_graphml();
