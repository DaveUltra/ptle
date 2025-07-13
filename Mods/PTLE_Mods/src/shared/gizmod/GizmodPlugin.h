#pragma once

#include <stdint.h>
#include <string>


class GizmodPlugin
{
	friend class Gizmod;

private:
	static GizmodPlugin* m_instance;
public:
	static inline GizmodPlugin* getInstance() { return m_instance; }

public:

	GizmodPlugin();

		/// Returns the path to the dedicated config directory for this mod.
	std::wstring getConfigDirectory() const;

		/// Get the version this mod was built for.
	std::string getVersionString() const;

	virtual void onEnable() { }


		/// The human-readable name of the mod, used in console and other user displays.
	virtual const char* getDisplayName() const = 0;

		/// The internal mod name, used for config, identification.
		/// It must only contain alphanumeric characters, dashes and underscores.
	virtual const char* getSystemName() const = 0;


private:

	uint32_t m_versionMajor;
	uint32_t m_versionMinor;
	uint32_t m_versionPatch;
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
