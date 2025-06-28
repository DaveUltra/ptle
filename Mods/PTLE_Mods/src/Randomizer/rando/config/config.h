#pragma once

#include <stdint.h>


struct config_field_type_t
{
	enum Enum
	{
		BOOLEAN,
		INT,
		LEVEL_CRC,
		STRING,
	};
};

struct config_field_t
{
	config_field_type_t::Enum m_type;
	const char* m_name;
	int m_offset;
};

#define CONFIG_FIELD( name, type ) { type, #name, offsetof(RandoConfig, name) },



struct RandoConfig
{
	uint32_t seed;                // Seed for random generation.
	uint32_t startingArea;        // Starting level CRC.

	bool legacy;                  // Legacy map generation, no linked entrances.
	bool entranceRando;

	bool randomizeShamanShop;     // Shuffle shaman prices.
	bool skipJaguar2;             // Warp straight to Pusca.
	bool skipWaterLevels;         // Remove Mysterious Temple, Flooded Cave and Twin Outposts cave.
	bool immediateSpiritFights;   // Transform Harry into the spirit immediately upon entering the temple.

	bool itemRandoInventory;      // Shuffle unlockable items as part of item rando.
	bool itemRandoIdols;          // Shuffle idols and explorers.

	// Default values.
	RandoConfig();
};

extern RandoConfig rando_config;



void load_config();

void display_config();
