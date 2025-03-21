#pragma once

#include <vector>
#include <map>
#include <stdint.h>


// Level exit, used to build transitions.
struct Exit
{
	uint32_t areaCRC;
	std::string areaName;
	std::vector<std::vector<std::string>> requires;

	Exit( uint32_t areaCRC, const std::string& areaName )
		: areaCRC( areaCRC ), areaName( areaName )
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
typedef std::map<std::string, std::vector<Area>> MajorAreas;

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


// Config.
struct RandoConfig
{
	uint32_t seed;
	uint32_t startingArea;
	bool legacy;
	bool randomizeShamanShop;
	bool skipJaguar2;
	bool skipWaterLevels;
	bool immediateSpiritFights;

	RandoConfig();
};

extern RandoConfig rando_config;



// Levels & transitions, loaded from "transition_infos.json".
extern MajorAreas transition_infos;

// Randomized transitions, as a map of (original transition) -> (override transition).
extern std::map<Transition, Transition> rando_transitions_map;



// Load configuration.
void rando_init();

void generate_randomized_map();

void randomize_shaman_shop();
void patch_shaman_shop();

void prevent_transition_softlock();

void write_graphml();

bool spoof_transition( Transition& t );

uint32_t* find_previous_area_memory();
