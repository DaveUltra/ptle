#include "rando/config/config.h"

#include "utils/log.h"
#include "ptle/levels/level_info.h"

#include <fstream>
#include <string>
#include <map>



// Config data.
RandoConfig rando_config;

// Runtime fields for automation.
static const config_field_t fields[] =
{
	// Entrance randomizer.
	CONFIG_FIELD( legacy,                config_field_type_t::BOOLEAN )
	CONFIG_FIELD( entranceRando,         config_field_type_t::BOOLEAN )

	// Seed + starting area.
	CONFIG_FIELD( seed,                  config_field_type_t::INT )
	CONFIG_FIELD( startingArea,          config_field_type_t::LEVEL_CRC )

	// Misc options.
	CONFIG_FIELD( randomizeShamanShop,   config_field_type_t::BOOLEAN )
	CONFIG_FIELD( skipJaguar2,           config_field_type_t::BOOLEAN )
	CONFIG_FIELD( skipWaterLevels,       config_field_type_t::BOOLEAN )
	CONFIG_FIELD( immediateSpiritFights, config_field_type_t::BOOLEAN )

	// Item randomizer.
	CONFIG_FIELD( itemRandoInventory,    config_field_type_t::BOOLEAN )
	CONFIG_FIELD( itemRandoIdols,        config_field_type_t::BOOLEAN )
};



static uint32_t parse_hex( const std::string& s )
{
	uint32_t v = 0;
	for ( char c : s ) {
		v *= 16;
		if ( c >= '0' && c <= '9' ) {
			v += (c - '0');
		}
		else if ( c >= 'a' && c <= 'f' ) {
			v += (c - 'a' + 10);
		}
		else if ( c >= 'A' && c <= 'F' ) {
			v += (c - 'A' + 10);
		}
	}
	return v;
}

static inline const char* bool_to_str(bool b)
{
	return b ? "true" : "false";
}



RandoConfig::RandoConfig()
	: seed( 0 )
	, startingArea( 0 )
	, legacy( false )
	, entranceRando( true )
	, randomizeShamanShop( true )
	, skipJaguar2( false )
	, skipWaterLevels( false )
	, immediateSpiritFights( false )
	, itemRandoInventory( false )
	, itemRandoIdols( false )
{

}

void load_config()
{
	std::ifstream cfg( "cfg/Randomizer/config.txt" );

	std::string line;
	std::getline(cfg, line);
	if ( line != "[RandomizerPC]" ) return;

	std::map<std::string, const config_field_t*> options;
	for ( int i = 0; i < _countof(fields); i++ ) {
		options.emplace( fields[i].m_name, &fields[i] );
	}

	while ( std::getline(cfg, line) ) {
		size_t eq = line.find( '=' );
		if ( line.empty() || eq == std::string::npos ) {
			continue;
		}

		std::string option = line.substr( 0, eq );
		std::string value = line.substr( eq + 1 );
		if ( value.empty() ) {
			continue;
		}

		auto it = options.find( option );
		if ( it == options.end() ) {
			continue;
		}

		switch ( it->second->m_type )
		{
		case config_field_type_t::BOOLEAN:
			((bool*) (((char*) &rando_config) + it->second->m_offset))[0] = (value == "true");
			break;

		case config_field_type_t::INT:
			((int*) (((char*) &rando_config) + it->second->m_offset))[0] = atoi(value.c_str());
			break;

		case config_field_type_t::LEVEL_CRC:
			((uint32_t*) (((char*) &rando_config) + it->second->m_offset))[0] = parse_hex(value);
			break;
		}
	}

	cfg.close();
}

void display_config()
{
	log_printf( "Rando config :\n" );
	for ( int i = 0; i < _countof(fields); i++ ) {
		switch ( fields[i].m_type )
		{
		case config_field_type_t::BOOLEAN:
			log_printf( "- %s = %s\n", fields[i].m_name, bool_to_str(((bool*) (((char*) &rando_config) + fields[i].m_offset))[0]) );
			break;

		case config_field_type_t::INT:
			log_printf( "- %s = %d\n", fields[i].m_name, ((int*) (((char*) &rando_config) + fields[i].m_offset))[0] );
			break;

		case config_field_type_t::LEVEL_CRC: {
			uint32_t crc = ((uint32_t*) (((char*) &rando_config) + fields[i].m_offset))[0];
			log_printf( "- %s = %s (0x%.8X)\n", fields[i].m_name, level_get_name(level_get_by_crc(crc)), crc );
			}
			break;
		}
	}
}
