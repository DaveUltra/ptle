#pragma once

#include <stdint.h>
#include <string>


class GizmodPlugin
{
private:
	static GizmodPlugin* m_instance;
public:
	static const uint32_t MODLOADER_VERSION;   // The associated mod loader version.
	static inline GizmodPlugin* getInstance() { return m_instance; }

public:

	GizmodPlugin();

		/// Returns the path to the dedicated config directory for this mod.
	std::wstring getConfigDirectory() const;

	virtual void onEnable() { }


		/// The human-readable name of the mod, used in console and other user displays.
	virtual const char* getDisplayName() const = 0;

		/// The internal mod name, used for config, identification.
		/// It must only contain alphanumeric characters, dashes and underscores.
	virtual const char* getSystemName() const = 0;
};



#include <Windows.h>
#define DECLARE_PLUGIN( className ) \
	extern "C" { \
		__declspec(dllexport) className pluginInstance; \
		__declspec(dllexport) class Pitfall* gizmodInstance = 0; \
	} \
	/* Stub function to avoid going around the mod loader. */ \
	BOOL WINAPI DllMain( HINSTANCE hinstace, DWORD reason, LPVOID ) \
	{ return true; }
