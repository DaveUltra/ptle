#pragma once

#include <stdint.h>


struct levels_t
{
	enum
	{
		ABANDONED_CAVERN,
		ALTAR_OF_AGES,
		ALTAR_OF_HUITACA,
		APU_ILLAPU_SHRINE,
		BATTERED_BRIDGE,
		BITTENBINDER_CAMP,
		BUTTERFLY_GLADE,
		CAVERN_LAKE,
		CHAMELEON_TEMPLE,
		COPACANTI_LAKE,
		CRASH_SITE,
		CRYSTAL_CAVERN,
		EKKEKO_ICE_CAVERN,
		EYES_OF_DOOM,
		FIREBOMBED_TOWERS,
		FLOODED_CAVE,
		FLOODED_COURTYARD,
		GATES_OF_EL_DORADO,
		GREAT_TREE,
		JUNGLE_CANYON,
		JUNGLE_TRAIL,
		LADDER_OF_MILES,
		MAMA_OULLO_TOWER,
		MONKEY_TEMPLE,
		MOUNTAIN_OVERLOOK,
		MOUNTAIN_SLED_RUN,
		MOUTH_OF_INTI,
		MYSTERIOUS_TEMPLE,
		NATIVE_JUNGLE,
		NATIVE_VILLAGE,
		PENGUIN_DEN,
		PENGUIN_TEMPLE,
		PLANE_COCKPIT,
		PUNCHAU_SHRINE,
		RENEGADE_FORT,
		RENEGADE_HEADQUARTERS,
		RUINS_OF_EL_DORADO,
		SCORPION_TEMPLE,
		SPINJA_LAIR,
		STATUES_OF_AYAR,
		ST_CLAIRE_EXCAVATION_CAMP,
		TELEPORT,
		TURTLE_MONUMENT,
		TWIN_OUTPOSTS,
		TWIN_OUTPOSTS_UNDERWATER,
		UNDERGROUND_DAM,
		VALLEY_OF_SPIRITS,
		VIRACOCHA_MONOLITHS,
		WHITE_VALLEY,
	};
};

struct level_info_t
{
	uint32_t crc;
	const char* name;
};

struct level_collection_t
{
	const level_info_t* levels;
	int count;
};


extern level_collection_t levels_game;
extern level_collection_t levels_beta;
