#include "level_info.h"
#include "ptle/containers/TreeMap/TreeMap.h"


#define count( arr ) (sizeof(arr) / sizeof(arr[0]))


static level_info_t level_info[] =
{
	{ 0xEA667977, "c07",     "Abandoned Cavern" },
	{ 0xABD7CCD8, "m01a",    "Altar of Ages" },
	{ 0x70EBFCA3, "a04",     "Altar of Huitaca" },
	{ 0xCFD2FE10, "a62",     "Ancient El Dorado (Pusca)" },
	{ 0x5511C46C, "s58b",    "Apu Illapu Shrine" },
	{ 0x69F0CDE2, "a14",     "Battered Bridge" },
	{ 0x47508760, "0-justin", "Beta Volcano" },
	{ 0x0EF63551, "m05",     "Bittenbinder's Camp" },
	{ 0xCD944049, "b62",     "Butterfly Glade" },
	{ 0x8B372E42, "s57",     "Cavern Lake" },
	{ 0x0081082C, "a01",     "Chameleon Temple" },
	{ 0x8147EA91, "s06",     "Copacanti Lake" },
	{ 0xEE8F6900, "a03",     "Crash Site" },
	{ 0x0DDE5470, "c09",     "Crystal Cavern" },
	{ 0x3E7EE822, "s60",     "Ekkeko Ice Cavern" },
	{ 0x9A0C8DF8, "c02",     "Eyes of Doom" },
	{ 0xF0F99C58, "a17",     "Fire-bombed Towers" },
	{ 0x3A811024, "b70",     "Flooded Cave" },
	{ 0xECC9D759, "b03",     "Flooded Courtyard" },
	{ 0x93F89D45, "a53",     "Gates of El Dorado" },
	{ 0x3292B6C9, "a202",    "Great Tree" },
	{ 0xDEDA69BC, "a100",    "Jungle Canyon" },
	{ 0x7A9B3870, "a55",     "Jungle Trail" },
	{ 0xE411440A, "o03",     "KaBOOM!" },
	{ 0x619E1126, "save_a3", "Ladder of Miles" },
	{ 0x07ECCC35, "a05",     "Mama-Oullo Tower" },
	{ 0x02C7B675, "b01",     "Monkey Temple (Spirit)" },
	{ 0xF3B4DC8E, "b01a",    "Monkey Temple (Harry)" },
	{ 0x907C492B, "c53",     "Mountain Overlook" },
	{ 0x1B8833D3, "s58",     "Mountain Sled Run" },
	{ 0xB8D5CE86, "a63",     "Mouth of Inti" },
	{ 0x099BF148, "m01",     "Mysterious Temple" },
	{ 0x0AF1CCFF, "a50",     "Native Jungle" },
	{ 0x05AA726C, "b05",     "Native Village" },
	{ 0x1553BBE1, "s50",     "Penguin Den" },
	{ 0x1F237F32, "s01",     "Penguin Temple (Spirit)" },
	{ 0x1B11EC74, "s01a",    "Penguin Temple (Harry)" },
	{ 0x7A75D1A9, "o04",     "Pickaxe Race" },
	{ 0x4A3E4058, "a03b",    "Plane Cockpit" },
	{ 0x53257119, "a02b",    "Plane Cutscene" },
	{ 0x38C7AE7D, "a70",     "Punchau Shrine" },
	{ 0x9316749C, "o02",     "Raft Bowling" },
	{ 0x0C1C3E47, "b09",     "Renegade Fort" },
	{ 0x1CB1432D, "b15",     "Renegade Headquarters" },
	{ 0x99885996, "a02",     "Ruins of El Dorado (Supai)" },
	{ 0x0305DC42, "c01",     "Scorpion Temple (Spirit)" },
	{ 0x4B08BBEB, "c01a",    "Scorpion Temple (Harry)" },
	{ 0x7652BAFC, "s14",     "Spinja Lair" },
	{ 0xA85A2793, "a202b",   "Statues of Ayar" },
	{ 0xBA9370DF, "b63",     "St. Claire's Excavation Camp (Day)" },
	{ 0x72AD42FA, "b04",     "St. Claire's Excavation Camp (Night)" },
	{ 0xE97CB47C, "save_warp_b_master", "Teleport" },
	{ 0x0D72E13F, "o05",     "Tuco Shoot" },
	{ 0x239A2165, "b60",     "Turtle Monument" },
	{ 0xE6B9138A, "b52",     "Twin Outposts" },
	{ 0xDE524DA6, "b52a",    "Twin Outposts (Underwater Cave)" },
	{ 0x9D6149E1, "c06",     "Underground Dam" },
	{ 0x08E3C641, "s19",     "Valley of the Spirits" },
	{ 0x6F498BBD, "s04",     "Viracocha Monoliths" },
	{ 0xE8362F5F, "s13",     "Viracocha Monoliths (Cutscene)" },
	{ 0x0A1F2526, "o01",     "Whack-A-Tuco" },
	{ 0x62548B77, "s51",     "White Valley" },
};

static level_info_t level_info_beta[] =
{
	{ 0xE9809967, "gym",      "gym" },
	{ 0x47508760, "0-justin", "0-justin (Beta Volcano Crash Site)" },
	{ 0x303E8832, "0-Brad",   "0-Brad (Beta Mama-Oullo Tower)" },
	{ 0x9FB1EC73, "0-Lacy",   "0-Lacy (Shaman Horde)" },
};


level_collection_t levels_game = { level_info, count(level_info) };
level_collection_t levels_beta = { level_info_beta, count(level_info_beta) };


uint32_t level_get_crc( uint32_t id )
{
	return level_info[id].crc;
}

const char* level_get_name( uint32_t id )
{
	if ( id >= levels_t::NUM_LEVELS ) {
		return "INVALID_LEVEL";
	}

	return level_info[id].displayName;
}

uint32_t level_get_by_crc( uint32_t crc )
{
	int size = count( level_info );
	for ( uint32_t i = 0; i < size; i++ ) {
		if ( level_info[i].crc == crc ) {
			return i;
		}
	}
	return -1;
}


const level_info_t* level_get_info( uint32_t id )
{
	if ( id >= levels_t::NUM_LEVELS ) {
		return 0;
	}

	return &level_info[id];
}

uint32_t* find_previous_area_memory()
{
	// This tree map contains a node which points to the previous area value...
	TreeMap* map = ((TreeMap*)0x91FE2C);

	// ... and this is the ID of said node.
	const uint32_t targetNodeID = 0x174CD628;

	// Look for it.
	// TODO : Binary search might be a good idea for performance.
	TreeMapNode* node = map->m_iterateFirst;
	while (node && node->m_hash != targetNodeID) {
		node = node->m_iterateNext;
	}

	if (node) {
		// This should be a pointer to and ESDInt.
		// Its value is at offset +0x08.
		uint32_t* scriptInt = (uint32_t*)node->m_ptr;
		return scriptInt + 2;
	}
	else {
		return 0;
	}
}
