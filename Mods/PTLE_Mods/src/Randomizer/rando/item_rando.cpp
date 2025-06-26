#include "item_rando.h"

#include "rando.h"

#include "ptle/EIHarry.h"
#include "injector/injector.hpp"
#include "utils/func.h"
#include "utils/log.h"

#include <map>
#include <vector>
#include <algorithm>



//
// Definitions.
//

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
};


struct less : public std::less<Unlockable*>
{
	bool operator()( Unlockable* const a, Unlockable* const b )
	{
		if ( a->m_type == b->m_type ) {
			return a->m_itemHash < b->m_itemHash;
		}
		else {
			return a->m_type < b->m_type;
		}
	}
};


static Unlockable g_unlockableItems[] =
{
	{ INVENTORY_ITEM, 0x915AA574, "Sling" },   // Sling.
	{ INVENTORY_ITEM, 0x04ADF9C7, "Torch" },   // Torch.
	{ INVENTORY_ITEM, 0xC024157C, "Pickaxes" },   // Pickaxes.
	{ INVENTORY_ITEM, 0x9608A818, "TNT" },   // TNT.
	{ INVENTORY_ITEM, 0xEE6B156E, "Shield" },   // Shield.
	{ INVENTORY_ITEM, 0xFB3D82AC, "Raft" },   // Raft.
	{ INVENTORY_ITEM, 0xCFC909C3, "Gas Mask" },   // Gas Mask.
	{ INVENTORY_ITEM, 0xE51C8F72, "Canteen" },   // Canteen.
	//{ INVENTORY_ITEM, 0x55A51DAB, "Stink Bomb" },   // Stink Bomb.
};

static std::map<Unlockable*, Unlockable*, less> g_unlockablesMap;

GET_METHOD( 0x506170, void, UnlockItem, void*, uint32_t );



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

static void _grant_unlockable( Unlockable* u )
{
	EIHarry* harry;

	switch ( u->m_type )
	{
	case INVENTORY_ITEM:
		harry = *((EIHarry**) 0x917034);
		UnlockItem( harry->m_itemHotbar, u->m_itemHash );
		break;
	}
}

static void _revoke_unlockable( Unlockable* u )
{
	switch ( u->m_type )
	{
	case INVENTORY_ITEM:
		_get_item_by_hash( u->m_itemHash )->m_unlocked = false;
		break;
	}
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

	_grant_unlockable( it->second );
}
MAKE_THISCALL_WRAPPER( UnlockItem_custom, _UnlockItem_custom );



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
	injector::MakeCALL( 0x4E9E51, UnlockItem_custom );   // Intercept item unlock.
	injector::MakeRangedNOP( 0x4E9D6F, 0x4E9D83 );       // Remove hotbar autoset for the first 4 items.
}
