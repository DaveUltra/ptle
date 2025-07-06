#include "gizmod/Gizmod.h"

#include "modloader/modloader.h"

#include <fstream>
#include <string>


void load_config()
{
	std::ifstream cfg( "./config.ini" );
	if ( !cfg ) {
		return;
	}

	std::string line;
	std::getline( cfg, line );
	if ( line != "[Gizmod]" ) {
		return;
	}

	while ( getline(cfg, line) ) {
		// Strip comment.
		size_t comment = line.find_first_of('#');
		if ( comment != std::string::npos ) {
			line = line.substr( 0, comment );
		}

		if ( line.empty() ) {
			continue;
		}

		// Option / value parsing.
		size_t eq = line.find( '=' );
		if ( eq == std::string::npos ) {
			continue;
		}

		std::string option = line.substr( 0, eq );
		std::string value = line.substr( eq + 1 );
		if ( value.empty() ) {
			continue;
		}

		if ( option == "enabled" ) {
			g_enabled = (value == "true");
		}
		else if ( option == "showConsole" ) {
			if ( value == "true" ) Gizmod::getInstance()->showConsole();
		}
	}
}
