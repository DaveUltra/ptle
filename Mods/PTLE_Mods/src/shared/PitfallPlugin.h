#pragma once

#include "utils/log.h"

#include <stdint.h>

class EIHarry;
class ERLevel;

class ILoadLevelEvent;


class PitfallPlugin
{
public:

	virtual void onEnable() { }

	virtual const char* getName() const = 0;


public:

	static EIHarry* getHarry();
	static ERLevel* getLevel();

	static uint32_t getCurrentLevelCRC();

	static void registerLoadLevel( ILoadLevelEvent* e );
};



#include <Windows.h>
#define DECLARE_PLUGIN( className ) \
	static className globalInstance; \
	BOOL WINAPI DllMain( HINSTANCE hinstace, DWORD reason, LPVOID ) \
	{ \
		switch ( reason ) \
		{ \
		case DLL_PROCESS_ATTACH: \
			log_printf( "%s : Starting...\n", globalInstance.getName() ); \
			globalInstance.onEnable(); \
			log_printf( "%s : Mod started.\n", globalInstance.getName() ); \
			break; \
		default: \
			break; \
		} \
		return true; \
	}
