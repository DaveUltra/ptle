#include "gizmod/Gizmod.h"
#include "PluginConfig.h"

#include "gizmod/GizmodPlugin.h"

#include <fstream>


PluginConfig::PluginConfig()
{

}

bool PluginConfig::parseDefaultConfigFile()
{
	return this->parseConfigFile( Gizmod::getThisPlugin()->getConfigDirectory() + L"/config.ini" );
}

bool PluginConfig::parseConfigFile( const std::wstring& path )
{
	m_options.clear();

	std::ifstream is( path );
	std::string line;

	if ( is.bad() ) return false;

	while ( std::getline(is, line) ) {
		// Strip comment.
		size_t comment = line.find_first_of( '#' );
		if ( comment != std::string::npos ) {
			line = line.substr( 0, comment );
		}

		size_t eq = line.find( '=' );
		if ( line.empty() || eq == std::string::npos ) {
			continue;
		}

		std::string option = line.substr( 0, eq );
		std::string value = line.substr( eq + 1 );
		if ( value.empty() ) {
			continue;
		}

		m_options.insert({ option, value });
	}

	is.close();
	return true;
}

const std::string& PluginConfig::getOptionString( const std::string& name, const std::string& defaultValue ) const
{
	auto it = m_options.find( name );
	if ( it != m_options.end() ) {
		return it->second;
	}
	return defaultValue;
}

bool PluginConfig::getOptionBoolean( const std::string& name, bool defaultValue ) const
{
	auto it = m_options.find( name );
	if ( it != m_options.end() ) {
		return it->second == "true";
	}
	return defaultValue;
}
