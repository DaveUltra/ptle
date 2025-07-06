#pragma once

#include "EventListener.h"
#include "Logger.h"

#include <string>

class GizmodPlugin;

class EIHarry;
class ERLevel;


class Gizmod
{
public:

	static const uint32_t VERSION_MAJOR = 0;
	static const uint32_t VERSION_MINOR = 1;
	static const uint32_t VERSION_PATCH = 0;

		/// Returns :
		/// - +2 if plugin was made for a later version.
		/// - -2 if plugin was made for an earlier version.
		/// - -1 if plugin was made for an earlier patch (still compatible).
		/// -  0 if versions perfectly match.
	static int checkVersion( GizmodPlugin* p );

public:

	Gizmod();

		/// Global mod loader instance.
	static Gizmod* getInstance();

		/// Global mod / plugin instance.
	static GizmodPlugin* getThisPlugin();

		/// Get the version of the mod loader.
	std::string getVersionString() const;

		/// Mod services.
	inline EventListener* getEventListener() { return &m_eventListener; }
	inline Logger* getLogger() { return &m_logger; }


	void showConsole();


		/// Util functions.
	static EIHarry* getHarry();
	static ERLevel* getLevel();

	static uint32_t getCurrentLevelCRC();


private:

	EventListener m_eventListener;
	Logger m_logger;
};
