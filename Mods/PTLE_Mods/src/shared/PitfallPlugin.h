#pragma once

#include "utils/log.h"

#include <stdint.h>
#include <string>


class EIHarry;
class ERLevel;

class ILoadLevelEvent;


class PitfallPlugin
{
private:
	static PitfallPlugin* m_instance;
public:
	static inline PitfallPlugin* getInstance() { return m_instance; }

public:

	PitfallPlugin();

	std::wstring getConfigDirectory() const;

	virtual void onEnable() { }

	virtual const char* getDisplayName() const = 0;
	virtual const char* getSystemName() const = 0;


	void log_printf( const char* fmt, ... );


public:

	static EIHarry* getHarry();
	static ERLevel* getLevel();

	static uint32_t getCurrentLevelCRC();

	template<typename EventType>
	static void registerEvent( typename EventType::ICallbackType* e )
	{
		EventType::registerCallback( e );
	}

	template<typename EventType>
	static void invokeEvent( EventType& e )
	{
		auto list = EventType::getCallbackList();
		for ( typename EventType::ICallbackType* c : list ) {
			c->
		}
	}

private:

	void log_prefix();

private:

	bool m_loggerNewLine;
};



#include <Windows.h>
#define DECLARE_PLUGIN( className ) \
	extern "C" { \
		__declspec(dllexport) className globalInstance; \
		__declspec(dllexport) class Pitfall* pitfallInstance = 0; \
	} \
	/* Stub function to avoid going around the mod loader. */ \
	BOOL WINAPI DllMain( HINSTANCE hinstace, DWORD reason, LPVOID ) \
	{ return true; }
