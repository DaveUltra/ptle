#include "item_rando.h"

#include "rando.h"

#include "ptle/EIHarry.h"
#include "ptle/EScriptContext.h"
#include "ptle/levels/level_info.h"

#include "injector/injector.hpp"
#include "utils/func.h"
#include "utils/log.h"

#include <map>
#include <vector>
#include <algorithm>



//
// Definitions.
//

GET_METHOD( 0x506170, void, UnlockItem, void*, uint32_t );
GET_FUNC( 0x4E9EC0, void, Script_HarryIsInInventory, EScriptContext* );
GET_FUNC( 0x65BDF0, int*, PopScriptVariable_Int, EScriptContext* );
GET_FUNC( 0x65C880, int*, GetOutVariable, EScriptContext* );

static ItemStruct* _get_item_by_hash( uint32_t itemHash );

enum UnlockableType
{
	INVENTORY_ITEM = 0,
	IDOL_SINGLE,
	IDOL_EXPLORER,
	ARTIFACT,
};

struct Unlockable
{
	UnlockableType m_type;
	uint32_t m_itemHash;
	const char* m_displayName;

	void grant()
	{
		EIHarry* harry;

		switch ( m_type )
		{
		case INVENTORY_ITEM:
			harry = *((EIHarry**) 0x917034);
			UnlockItem( harry->m_itemHotbar, m_itemHash );
			break;
		}
	}

	void revoke()
	{
		switch ( m_type )
		{
		case INVENTORY_ITEM:
			_get_item_by_hash( m_itemHash )->m_unlocked = false;
			break;
		}
	}
};


static Unlockable g_unlockableItems[] =
{
	{ INVENTORY_ITEM, 0x915AA574, "Sling" },
	{ INVENTORY_ITEM, 0x04ADF9C7, "Torch" },
	{ INVENTORY_ITEM, 0xC024157C, "Pickaxes" },
	{ INVENTORY_ITEM, 0x9608A818, "TNT" },
	{ INVENTORY_ITEM, 0xEE6B156E, "Shield" },
	{ INVENTORY_ITEM, 0xFB3D82AC, "Raft" },
	{ INVENTORY_ITEM, 0xCFC909C3, "Gas Mask" },
	{ INVENTORY_ITEM, 0xE51C8F72, "Canteen" },
	//{ INVENTORY_ITEM, 0x55A51DAB, "Stink Bomb" },
};


// Global item rando mapping.
struct less : public std::less<Unlockable*> {
	bool operator()( Unlockable* const a, Unlockable* const b ) const {
		if ( a->m_type == b->m_type ) {
			return a->m_itemHash < b->m_itemHash;
		}
		else {
			return a->m_type < b->m_type;
		}
	}
};
static std::map<Unlockable*, Unlockable*, less> g_unlockablesMap;



//
// Code injection section.
//

static ItemStruct* _get_item_by_hash( uint32_t itemHash )
{
	ItemStruct* items = (ItemStruct*) 0x8EEB90;

	for ( int i = 0; i < 9; i++ ) {
		if ( items[i].m_hash == itemHash ) {
			return items + i;
		}
	}
	return 0;
}

static void _UnlockItem_custom( void* self, uint32_t itemHash )
{
	// Find override.
	Unlockable u = { INVENTORY_ITEM, itemHash };
	auto it = g_unlockablesMap.find( &u );

	// No override, just unlock the item as intended.
	if ( it == g_unlockablesMap.end() ) {
		UnlockItem( self, itemHash );
		return;
	}

	it->second->grant();
}
MAKE_THISCALL_WRAPPER( UnlockItem_custom, _UnlockItem_custom );

static void Script_HarryIsInInventory_custom( EScriptContext* context )
{
	uint32_t currentAreaCRC = *((uint32_t*) 0x917088);

	// Plane cockpit cutscene checks if we have canteen, just spoof the answer with "no".
	if ( currentAreaCRC == levelCRC::PLANE_COCKPIT ) {
		PopScriptVariable_Int( context );
		int* out = GetOutVariable( context );

		*out = 0;
	}
	else {
		Script_HarryIsInInventory( context );
	}
}



//
// Initialization.
//

void item_rando_init()
{
	// Stage item categories and shuffle them.
	std::vector<Unlockable*> original;
	std::vector<Unlockable*> shuffled;

	if ( rando_config.itemRandoInventory ) {
		for ( int i = 0; i < _countof(g_unlockableItems); i++ ) {
			original.push_back( g_unlockableItems + i );
		}
	}

	// No item rando option is on, so we don't need to enable anything here.
	if ( original.empty() ) {
		return;
	}

	shuffled = original;
	std::random_shuffle( shuffled.begin(), shuffled.end() );

	log_printf( "Item Rando :\n" );
	for ( int i = 0; i < shuffled.size(); i++ ) {
		g_unlockablesMap.emplace( original[i], shuffled[i] );

		log_printf( "- %s  -->  %s\n", original[i]->m_displayName, shuffled[i]->m_displayName );
	}


	// Code injection.
	injector::MakeCALL( 0x4E9E51, UnlockItem_custom );   // Intercept item unlock ("HarryAddInventoryItem" script function).
	injector::MakeCALL( 0x598036, UnlockItem_custom );   // Intercept item unlock (picking up an EITreasure).
	injector::MakeRangedNOP( 0x4E9D6F, 0x4E9D83 );       // Remove hotbar autoset for the first 4 items.
	injector::WriteMemory( 0x8EF35C, Script_HarryIsInInventory_custom );   // Disable Plane Cockpit's check for canteen.
}
