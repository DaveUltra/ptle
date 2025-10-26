#pragma once

#include <string>
#include <map>


	/// Standardized configuration file parser for plugins.
	/// - Boolean rules :
	///   - A value is true if the option string is "true". All other options are considered false.
	/// - Comments :
	///   - If any '#' is met on a line, everything past it will be ignored.
class PluginConfig
{
public:

	PluginConfig();

		/// Fills this configuration with the contents of "config/[PluginName]/config.ini".
	bool parseDefaultConfigFile();

		/// Parse any file.
	bool parseConfigFile( const std::wstring& path );

		/// Get option values.
	const std::string& getOptionString( const std::string& name, const std::string& defaultValue ) const;
	bool getOptionBoolean( const std::string& name, bool defaultValue ) const;


private:

	std::map<std::string, std::string> m_options;
};
