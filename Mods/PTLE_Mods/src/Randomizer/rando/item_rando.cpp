#include "item_rando.h"

#include "rando.h"

#include "ptle/EIHarry.h"
#include "ptle/EScriptContext.h"
#include "ptle/EITreasureIdol.h"
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
GET_FUNC( 0x5E34A0, void, AddCollectedIdols, uint32_t, int );
GET_FUNC( 0x4E9EC0, void, Script_HarryIsInInventory, EScriptContext* );
GET_FUNC( 0x65BDF0, int*, PopScriptVariable_Int, EScriptContext* );
GET_FUNC( 0x65C880, int*, GetOutVariable, EScriptContext* );

static ItemStruct* _get_item_by_hash( uint32_t itemHash );

enum UnlockableType
{
	INVENTORY_ITEM = 0,  // Harry's items (canteen, sling, etc...)
	IDOL_SINGLE,         // Idols in totems.
	IDOL_EXPLORER,       // Idols given by explorers.
	IDOL_RAFT,           // Special idol in Mountain Sled Run.
	ARTIFACT,            // Temple artifacts.
};

struct Unlockable
{
	UnlockableType m_type;
	union {
		uint32_t m_itemHash;
		const Idol* m_idol;
	};
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
		case IDOL_SINGLE:
			AddCollectedIdols( m_idol->m_levelCRC, 1 );
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

static std::vector<Unlockable> g_unlockableIdols;


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
	// St.Claire night time : Don't randomize items!
	uint32_t currentAreaCRC = *((uint32_t*) 0x917088);
	if ( currentAreaCRC == levelCRC::ST_CLAIRE_EXCAVATION_CAMP_NIGHT ) {
		UnlockItem( self, itemHash );
		return;
	}

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

// VERY hacky :
// The original "AddCollectedIdols" function is not a member function. But at the injection point, ECX contains a pointer
// to the idol, which we can retrieve by pretending that it is a member function.
// In the process, we override "levelCRC" and "amount", but their values are always known.
static void _AddCollectedIdols_custom( EITreasureIdol* self, uint32_t levelCRC, int amount )
{
	levelCRC = *((uint32_t*) 0x917088);

	// Find override.
	const Idol* idol = 0;
	for ( const Idol& i : idols_info[levelCRC] ) {
		if ( i.m_uniqueID == self->m_uniqueID ) {
			idol = &i;
			break;
		}
	}
	if ( idol == 0 ) {
		log_printf( "No idol found! Was idols_infos.json loaded correctly?\n" );
	}

	Unlockable u = { IDOL_SINGLE, (uint32_t) idol };
	auto it = g_unlockablesMap.find( &u );

	// No override.
	if ( it == g_unlockablesMap.end() ) {
		AddCollectedIdols( levelCRC, 1 );
		return;
	}

	it->second->grant();
}
MAKE_THISCALL_WRAPPER( AddCollectedIdols_custom, _AddCollectedIdols_custom );

static void Script_HarryIsInInventory_custom( EScriptContext* context )
{
	uint32_t currentAreaCRC = *((uint32_t*) 0x917088);

	// Plane cockpit cutscene checks if we have canteen, just spoof the answer with "no".
	if ( currentAreaCRC == levelCRC::PLANE_COCKPIT ) {
		PopScriptVariable_Int( context );
		int* out = GetOutVariable( context );

		*out = 0;
	}
	// Native Village's shield cutscene will be gone if we enter with shield already.
	else if ( currentAreaCRC == levelCRC::NATIVE_VILLAGE ) {
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

	if ( rando_config.itemRandoIdols ) {
		for ( auto& idolsList : idols_info ) {
			for ( const Idol& idol : idolsList.second ) {
				Unlockable u = { IDOL_SINGLE, (uint32_t) &idol, "Idol" };
				g_unlockableIdols.push_back( u );
			}
		}

		for ( Unlockable& u : g_unlockableIdols ) {
			original.push_back( &u );
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
		if ( shuffled[i]->m_type == INVENTORY_ITEM && original[i]->m_type == IDOL_SINGLE ) {
			log_printf( "  - In %s\n", level_get_name(level_get_by_crc(original[i]->m_idol->m_levelCRC)) );
		}
	}


	// Code injection.
	injector::MakeCALL( 0x4E9E51, UnlockItem_custom );        // Intercept item unlock ("HarryAddInventoryItem" script function).
	injector::MakeCALL( 0x598036, UnlockItem_custom );        // Intercept item unlock (picking up an EITreasure).
	injector::MakeCALL( 0x598004, AddCollectedIdols_custom ); // Intercept idol grab.
	injector::MakeRangedNOP( 0x597FE8, 0x597FFF );
	injector::MakeNOP( 0x598009, 3 );
	injector::MakeRangedNOP( 0x4E9D6F, 0x4E9D83 );            // Remove hotbar autoset for the first 4 items.
	injector::WriteMemory( 0x8EF35C, Script_HarryIsInInventory_custom );   // Disable Plane Cockpit's check for canteen.
}
