#pragma once

#include "EventListener.h"
#include "Logger.h"
#include "save/SaveManager.h"
#include "item/InventoryItem.h"

#include <string>

class GizmodPlugin;

class EIHarry;
class ERLevel;
class EUIHud;


class Gizmod
{
public:

	static const uint32_t VERSION_MAJOR = 0;
	static const uint32_t VERSION_MINOR = 1;
	static const uint32_t VERSION_PATCH = 1;

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
	inline SaveManager* getSaveManager() { return &m_saveManager; }

	inline InventoryItem* getInventoryItem( InventoryItem::Item i ) { return i == InventoryItem::UNKNOWN ? 0 : &m_inventoryItems[i]; };


	void showConsole();


		/// Util functions.
	static EIHarry* getHarry();
	static ERLevel* getLevel();
	static EUIHud* getHUD();

	static uint32_t getCurrentLevelCRC();


private:

	EventListener m_eventListener;
	Logger m_logger;
	SaveManager m_saveManager;

	InventoryItem m_inventoryItems[8];
};
