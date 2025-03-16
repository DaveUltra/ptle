#include "level_info.h"


#define count( arr ) (sizeof(arr) / sizeof(arr[0]))


static level_info_t level_info[] =
{
	{ 0xEA667977, "Abandoned Cavern" },
	{ 0xABD7CCD8, "Altar of Ages" },
	{ 0x70EBFCA3, "Altar of Huitaca" },
	{ 0xCFD2FE10, "Ancient El Dorado (Pusca)" },
	{ 0x5511C46C, "Apu Illapu Shrine" },
	{ 0x69F0CDE2, "Battered Bridge" },
	{ 0x47508760, "Beta Volcano" },
	{ 0x0EF63551, "Bittenbinder's Camp" },
	{ 0xCD944049, "Butterfly Glade" },
	{ 0x8B372E42, "Cavern Lake" },
	{ 0x0081082C, "Chameleon Temple" },
	{ 0x8147EA91, "Copacanti Lake" },
	{ 0xEE8F6900, "Crash Site" },
	{ 0x0DDE5470, "Crystal Cavern" },
	{ 0x3E7EE822, "Ekkeko Ice Cavern" },
	{ 0x9A0C8DF8, "Eyes of Doom" },
	{ 0xF0F99C58, "Fire-bombed Towers" },
	{ 0x3A811024, "Flooded Cave" },
	{ 0xECC9D759, "Flooded Courtyard" },
	{ 0x93F89D45, "Gates of El Dorado" },
	{ 0x3292B6C9, "Great Tree" },
	{ 0xDEDA69BC, "Jungle Canyon" },
	{ 0x7A9B3870, "Jungle Trail" },
	{ 0xE411440A, "KaBOOM!" },
	{ 0x619E1126, "Ladder of Miles" },
	{ 0x07ECCC35, "Mama-Oullo Tower" },
	{ 0x02C7B675, "Monkey Temple (Spirit)" },
	{ 0xF3B4DC8E, "Monkey Temple (Harry)" },
	{ 0x907C492B, "Mountain Overlook" },
	{ 0x1B8833D3, "Mountain Sled Run" },
	{ 0xB8D5CE86, "Mouth of Inti" },
	{ 0x099BF148, "Mysterious Temple" },
	{ 0x0AF1CCFF, "Native Jungle" },
	{ 0x05AA726C, "Native Village" },
	{ 0x1553BBE1, "Penguin Den" },
	{ 0x1F237F32, "Penguin Temple (Spirit)" },
	{ 0x1B11EC74, "Penguin Temple (Harry)" },
	{ 0x7A75D1A9, "Pickaxe Race" },
	{ 0x4A3E4058, "Plane Cockpit" },
	{ 0x53257119, "Plane Cutscene" },
	{ 0x38C7AE7D, "Punchau Shrine" },
	{ 0x9316749C, "Raft Bowling" },
	{ 0x0C1C3E47, "Renegade Fort" },
	{ 0x1CB1432D, "Renegade Headquarters" },
	{ 0x99885996, "Ruins of El Dorado (Supai)" },
	{ 0x0305DC42, "Scorpion Temple (Spirit)" },
	{ 0x4B08BBEB, "Scorpion Temple (Harry)" },
	{ 0x7652BAFC, "Spinja Lair" },
	{ 0xA85A2793, "Statues of Ayar" },
	{ 0xBA9370DF, "St. Claire's Excavation Camp (Day)" },
	{ 0x72AD42FA, "St. Claire's Excavation Camp (Night)" },
	{ 0xE97CB47C, "Teleport" },
	{ 0x0D72E13F, "Tuco Shoot" },
	{ 0x239A2165, "Turtle Monument" },
	{ 0xE6B9138A, "Twin Outposts" },
	{ 0xDE524DA6, "Twin Outposts (Underwater Cave)" },
	{ 0x9D6149E1, "Underground Dam" },
	{ 0x08E3C641, "Valley of the Spirits" },
	{ 0x6F498BBD, "Viracocha Monoliths" },
	{ 0xE8362F5F, "Viracocha Monoliths (Cutscene)" },
	{ 0x0A1F2526, "Whack-A-Tuco" },
	{ 0x62548B77, "White Valley" },
};

static level_info_t level_info_beta[] =
{
	{ 0xE9809967, "gym" },
	{ 0x47508760, "0-justin (Beta Volcano Crash Site)" },
	{ 0x303E8832, "0-Brad (Beta Mama-Oullo Tower)" },
	{ 0x9FB1EC73, "0-Lacy (Shaman Horde)" },
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

	return level_info[id].name;
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
