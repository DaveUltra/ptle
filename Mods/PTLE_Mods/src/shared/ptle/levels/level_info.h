#pragma once

#include <stdint.h>


struct levels_t {
	enum Enum {
		ABANDONED_CAVERN,
		ALTAR_OF_AGES,
		ALTAR_OF_HUITACA,
		ANCIENT_EL_DORADO,
		APU_ILLAPU_SHRINE,
		BATTERED_BRIDGE,
		BETA_VOLCANO,
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
		KABOOM,
		LADDER_OF_MILES,
		MAMA_OULLO_TOWER,
		MONKEY_SPIRIT,
		MONKEY_TEMPLE,
		MOUNTAIN_OVERLOOK,
		MOUNTAIN_SLED_RUN,
		MOUTH_OF_INTI,
		MYSTERIOUS_TEMPLE,
		NATIVE_JUNGLE,
		NATIVE_VILLAGE,
		PENGUIN_DEN,
		PENGUIN_SPIRIT,
		PENGUIN_TEMPLE,
		PICKAXE_RACE,
		PLANE_COCKPIT,
		PLANE_CUTSCENE,
		PUNCHAU_SHRINE,
		RAFT_BOWLING,
		RENEGADE_FORT,
		RENEGADE_HEADQUARTERS,
		RUINS_OF_EL_DORADO,
		SCORPION_SPIRIT,
		SCORPION_TEMPLE,
		SPINJA_LAIR,
		STATUES_OF_AYAR,
		ST_CLAIRE_EXCAVATION_CAMP_DAY,
		ST_CLAIRE_EXCAVATION_CAMP_NIGHT,
		TELEPORT,
		TUCO_SHOOT,
		TURTLE_MONUMENT,
		TWIN_OUTPOSTS,
		TWIN_OUTPOSTS_UNDERWATER,
		UNDERGROUND_DAM,
		VALLEY_OF_SPIRITS,
		VIRACOCHA_MONOLITHS,
		VIRACOCHA_MONOLITHS_CUTSCENE,
		WHACK_A_TUCO,
		WHITE_VALLEY,

		NUM_LEVELS
	};
};

struct levelCRC {
	enum Enum {
		ABANDONED_CAVERN             = 0xEA667977,
		ALTAR_OF_AGES                = 0xABD7CCD8,
		ALTAR_OF_HUITACA             = 0x70EBFCA3,
		ANCIENT_EL_DORADO            = 0xCFD2FE10,
		APU_ILLAPU_SHRINE            = 0x5511C46C,
		BATTERED_BRIDGE              = 0x69F0CDE2,
		BETA_VOLCANO                 = 0x47508760,
		BITTENBINDER_CAMP            = 0x0EF63551,
		BUTTERFLY_GLADE              = 0xCD944049,
		CAVERN_LAKE                  = 0x8B372E42,
		CHAMELEON_TEMPLE             = 0x0081082C,
		COPACANTI_LAKE               = 0x8147EA91,
		CRASH_SITE                   = 0xEE8F6900,
		CRYSTAL_CAVERN               = 0x0DDE5470,
		EKKEKO_ICE_CAVERN            = 0x3E7EE822,
		EYES_OF_DOOM                 = 0x9A0C8DF8,
		FIREBOMBED_TOWERS            = 0xF0F99C58,
		FLOODED_CAVE                 = 0x3A811024,
		FLOODED_COURTYARD            = 0xECC9D759,
		GATES_OF_EL_DORADO           = 0x93F89D45,
		GREAT_TREE                   = 0x3292B6C9,
		JUNGLE_CANYON                = 0xDEDA69BC,
		JUNGLE_TRAIL                 = 0x7A9B3870,
		KABOOM                       = 0xE411440A,
		LADDER_OF_MILES              = 0x619E1126,
		MAMA_OULLO_TOWER             = 0x07ECCC35,
		MONKEY_SPIRIT                = 0x02C7B675,
		MONKEY_TEMPLE                = 0xF3B4DC8E,
		MOUNTAIN_OVERLOOK            = 0x907C492B,
		MOUNTAIN_SLED_RUN            = 0x1B8833D3,
		MOUTH_OF_INTI                = 0xB8D5CE86,
		MYSTERIOUS_TEMPLE            = 0x099BF148,
		NATIVE_JUNGLE                = 0x0AF1CCFF,
		NATIVE_VILLAGE               = 0x05AA726C,
		PENGUIN_DEN                  = 0x1553BBE1,
		PENGUIN_SPIRIT               = 0x1F237F32,
		PENGUIN_TEMPLE               = 0x1B11EC74,
		PICKAXE_RACE                 = 0x7A75D1A9,
		PLANE_COCKPIT                = 0x4A3E4058,
		PLANE_CUTSCENE               = 0x53257119,
		PUNCHAU_SHRINE               = 0x38C7AE7D,
		RAFT_BOWLING                 = 0x9316749C,
		RENEGADE_FORT                = 0x0C1C3E47,
		RENEGADE_HEADQUARTERS        = 0x1CB1432D,
		RUINS_OF_EL_DORADO           = 0x99885996,
		SCORPION_SPIRIT              = 0x0305DC42,
		SCORPION_TEMPLE              = 0x4B08BBEB,
		SPINJA_LAIR                  = 0x7652BAFC,
		STATUES_OF_AYAR              = 0xA85A2793,
		ST_CLAIRE_EXCAVATION_CAMP_DAY = 0xBA9370DF,
		ST_CLAIRE_EXCAVATION_CAMP_NIGHT = 0x72AD42FA,
		TELEPORT                     = 0xE97CB47C,
		TUCO_SHOOT                   = 0x0D72E13F,
		TURTLE_MONUMENT              = 0x239A2165,
		TWIN_OUTPOSTS                = 0xE6B9138A,
		TWIN_OUTPOSTS_UNDERWATER     = 0xDE524DA6,
		UNDERGROUND_DAM              = 0x9D6149E1,
		VALLEY_OF_SPIRITS            = 0x08E3C641,
		VIRACOCHA_MONOLITHS          = 0x6F498BBD,
		VIRACOCHA_MONOLITHS_CUTSCENE = 0xE8362F5F,
		WHACK_A_TUCO                 = 0x0A1F2526,
		WHITE_VALLEY                 = 0x62548B77,
	};
};


struct level_info_t
{
	uint32_t crc;
	const char* assetName;
	const char* displayName;
};

struct level_collection_t
{
	const level_info_t* levels;
	int count;
};


extern level_collection_t levels_game;
extern level_collection_t levels_beta;


uint32_t level_get_crc( uint32_t id );
const char* level_get_name( uint32_t id );
uint32_t level_get_by_crc( uint32_t crc );

const level_info_t* level_get_info( uint32_t id );


#include "utils/func.h"

GET_FUNC( 0x6F8EC0, uint32_t, level_HashString, char* );

uint32_t* find_previous_area_memory();
