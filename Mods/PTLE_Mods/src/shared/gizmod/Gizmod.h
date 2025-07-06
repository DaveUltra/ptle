#pragma once

#include "EventListener.h"
#include "Logger.h"

class GizmodPlugin;

class EIHarry;
class ERLevel;


class Gizmod
{
public:

	static const uint32_t VERSION = 0;

public:

	Gizmod();

		/// Global mod loader instance.
	static Gizmod* getInstance();

		/// Global mod / plugin instance.
	static GizmodPlugin* getThisPlugin();

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
