#include "command_palette.h"

#include "injector/injector.hpp"

#include "ptle/EIHarry.h"
#include "ptle/EIProjectile.h"
#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"

#include "ptle/levels/level_info.h"
#include "ptle/types/types.h"

#include "ptle/containers/TreeMap/TreeMap.h"

#include "utils/log.h"
#include "utils/func.h"



GET_METHOD( 0x5EBA90, void, ScheduleWorldLoad, void*, uint32_t, bool );

GET_FUNC( 0x557840, EIProjectile*, EIProjectile_Create, uint32_t );
GET_METHOD( 0x559640, void, EIProjectile_Spawn, EIProjectile*, Vector3f*, Vector3f*, EInstance*, float );



static void _empty()
{

}


// --------------------------------------------------------------------------------
// Give useful items, abilities, and max HP.
// --------------------------------------------------------------------------------
static void _give_stuff()
{
	EIHarry* harry = injector::ReadMemory<EIHarry*>( 0x917034 );
	if ( !harry || !harry->m_itemHotbar ) {
		log_printf( "Harry's not spawned yet.\n" );
		return;
	}

	// All abilities (expect dive & super sling).
	harry->m_risingStrike = 1;
	harry->m_heroicDash = 1;
	harry->m_smashStrike = 1;
	harry->m_breakdance = 1;

	// Max HP (10).
	harry->m_healthMax = 10;
	harry->m_healthValue = 10;

	// Hotbar.
	harry->m_itemHotbar[0] = injector::ReadMemory<void*>( 0x8EEC20 );  // Raft.
	harry->m_itemHotbar[1] = injector::ReadMemory<void*>( 0x8EEBCC );  // Pickaxes.
	harry->m_itemHotbar[2] = injector::ReadMemory<void*>( 0x8EEBB0 );  // Torch.
	harry->m_itemHotbar[3] = injector::ReadMemory<void*>( 0x8EEBE8 );  // TNT.

	// Unlock all items.
	ItemStruct* items = (ItemStruct*) 0x8EEB90;
	for ( int i = 0; i < 8; i++ ) {
		items[i].m_unlocked = 1;
	}

	// Item swap (true = no swap, false = swap enabled).
	bool* itemSwapPtr = (bool*) 0x9106A2;
	*itemSwapPtr = false;
}


// --------------------------------------------------------------------------------
// Max out Harry's HP (and set it to game maximum, 10).
// --------------------------------------------------------------------------------
static void _heal()
{
	EIHarry* harry = injector::ReadMemory<EIHarry*>( 0x917034 );
	if ( !harry ) {
		log_printf( "Harry's not spawned yet.\n" );
		return;
	}

	harry->m_healthMax = 10;
	harry->m_healthValue = 10;
}


// --------------------------------------------------------------------------------
// Kill Harry.
// --------------------------------------------------------------------------------
static void _die()
{
	EIHarry* harry = injector::ReadMemory<EIHarry*>( 0x917034 );
	if ( !harry ) return;

	harry->m_healthValue = 0;
}


// --------------------------------------------------------------------------------
// TNT rain.
// --------------------------------------------------------------------------------
static EIProjectile* _do_tnt_rain( uint32_t code )
{
	Vector3f pos, dir;
	EIHarry* harry = injector::ReadMemory<EIHarry*>( 0x917034 );

	for ( int i = -12; i <= 12; i++ ) {
		for ( int j = -12; j <= 12; j++ ) {
			dir.x = dir.y = dir.z = 0.0F;

			pos.z = 70.0F;
			pos.x = j * 16.0F;
			pos.y = i * 16.0F;

			EIProjectile* tnt = EIProjectile_Create( 0xBBCAC492 );
			EIProjectile_Spawn( tnt, &pos, &dir, harry, 1.0F );
		}
	}

	return EIProjectile_Create( code );
}

static void _tnt_rain()
{
	injector::MakeCALL( 0x59540E, _do_tnt_rain );
}


// --------------------------------------------------------------------------------
// Every projectile is a TNT.
// --------------------------------------------------------------------------------
static void _all_tnt()
{
	// mov ecx, BBCAC492
	uint8_t bytes[] = { 0xC7, 0x45, 0x08, 0x92, 0xC4, 0xCA, 0xBB };
	injector::WriteMemoryRaw( 0x55784E, bytes, sizeof(bytes), true );
}


// --------------------------------------------------------------------------------
// Natives tick faster.
// --------------------------------------------------------------------------------
GET_METHOD( 0x414F80, void, EINative_Tick, EInstance* );

void _native_tick_loop( EInstance* inst )
{
	for ( int i = 0; i < 3; i++ ) {
		EINative_Tick( inst );
	}
}

MAKE_THISCALL_WRAPPER( _native_func, _native_tick_loop );
static void _fast_native()
{
	void* func = &_native_func;
	injector::WriteMemoryRaw( 0x88B014, &func, 4, true );
}


// --------------------------------------------------------------------------------
// Idol spawner.
// --------------------------------------------------------------------------------
#include "ptle/EITreasureIdol.h"
#include "ptle/ERLevel.h"
GET_METHOD( 0x626670, void, AddToWorld, ERLevel*, EInstance* );
GET_METHOD( 0x6265A0, void, AddToWorldFancy, ERLevel*, EInstance*, EInstance*, bool );
GET_METHOD( 0x6268D0, void, ERLevel_SetupEntity, ERLevel*, EInstance*, EInstance* );
GET_METHOD( 0x597400, void, InitIdol, EITreasureIdol*, Vector3f*, Vector4f*, uint32_t, uint32_t, uint32_t );
GET_METHOD( 0x547160, void, SetSomethingForHarry, EIHarry*, void* );
GET_METHOD( 0x4AD5E0, void, HarryInit, EIHarry*, Vector3f*, float );
static void _idol_spawn()
{
	const type_info_t* type = get_type_by_vtable( 0x89A2F8 );
	EITreasureIdol* idol = instantiate_object<EITreasureIdol>( type );

	EIHarry* harry = *((EIHarry**) 0x917034);

	Vector3f pos = { 0, 0, 0 };
	Vector4f rot = { 0, 0, 0, 1 };

	memcpy( &pos, &harry->m_position, sizeof(Vector3f) );

	InitIdol( idol, &pos, &rot, 0x816D97BF, 0x14F14DDC, 0x6061A9B3 );

	AddToWorld( harry->m_world, idol );
	AddToWorldFancy( harry->m_world, idol->m_particleEmitter, idol, false );
	ERLevel_SetupEntity( harry->m_world, idol, 0 );
}


// --------------------------------------------------------------------------------
// Harry spawner (unstable).
// --------------------------------------------------------------------------------
static EIProjectile* _harry_spawn( uint32_t code )
{
	EIHarry** harryPtr = ((EIHarry**) 0x917034);
	EIHarry* harry = *harryPtr;

	// Disable HUD updates (maybe?)
	injector::MakeRET( 0x4B0950 );

	// Don't create global items again.
	injector::WriteMemory<uint8_t>( 0x4AEA29, 0xEB );

	// No item hotbar.
	uint8_t bytes[10] = { 0xC7, 0x81, 0x3C, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	injector::MakeRangedNOP( 0x4AEA67, 0x4AEA7E );
	injector::WriteMemoryRaw( 0x4AEA67, bytes, 10, true );

	// Instantiate a new Harry.
	const type_info_t* type = get_type_by_vtable( 0x87D300 );
	EIHarry* newHarry = instantiate_object<EIHarry>( type );

	// Init.
	{
		//void* ptr = *((void**) 0x9170A4);
		//SetSomethingForHarry( newHarry, ptr );

		// /!\ This duplicates items (CRASH)
		Vector3f pos = { 0, 0, 0 };
		HarryInit( newHarry, &pos, 0.0F );

		newHarry->m_world = 0;
		AddToWorld( harry->m_world, newHarry );
		newHarry->m_world = harry->m_world;

		newHarry->m_itemHotbar = harry->m_itemHotbar;
		//*(((uint8_t*) newHarry) + 0x3e3) = false;
	}

	return EIProjectile_Create( code );
}

#include "ptle/EINative.h"
GET_FUNC( 0x6F8EC0, uint32_t, HashPath, char* );

GET_METHOD( 0x5242A0, void, EINative_Init, EINative* );
GET_METHOD( 0x412850, void, EIBeast_LoadBeastAsset, EIBeast* );
GET_METHOD( 0x527E50, int,  EIBeast_Deserialize2, EIBeast* );
static void _netiv_spawn()
{
	EIHarry** harryPtr = ((EIHarry**) 0x917034);
	EIHarry* harry = *harryPtr;

	const type_info_t* type = get_type_by_vtable( 0x88AFE8 );
	EINative* native = instantiate_object<EINative>( type );

	native->m_position.x = harry->m_position.x;
	native->m_position.y = harry->m_position.y;
	native->m_position.z = harry->m_position.z;

	native->m_rotation.x = 0.0F;
	native->m_rotation.y = 0.0F;
	native->m_rotation.z = 0.0F;
	native->m_rotation.w = 1.0F;

	uint32_t hash = HashPath( "BNative" );
	native->m_beastTypeCRC = hash;
	native->m_beastTypeName = "native";
	EIBeast_LoadBeastAsset( native );

	EIBeast_Deserialize2( native );

	if ( native->m_beastType ) {
		log_printf( "Beast type asset load failed.\n" );
	}

	ERLevel_SetupEntity( harry->m_world, native, 0 );
	EINative_Init( native );

	AddToWorld( harry->m_world, native );
}

#include "ptle/EINPCBeast.h"

GET_METHOD( 0x527ED0, void, EINPCBeast_Init, EINPCBeast* );

static void _micay_spawn()
{
	EIHarry** harryPtr = ((EIHarry**) 0x917034);
	EIHarry* harry = *harryPtr;

	const type_info_t* type = get_type_by_vtable( 0x88B868 );
	EINPCBeast* native = instantiate_object<EINPCBeast>( type );

	native->m_position.x = harry->m_position.x;
	native->m_position.y = harry->m_position.y;
	native->m_position.z = harry->m_position.z;

	native->m_rotation.x = 0.0F;
	native->m_rotation.y = 0.0F;
	native->m_rotation.z = 0.0F;
	native->m_rotation.w = 1.0F;

	//"micay", "bernard", "chief", "leech", "nicoleWchute", "nicole"
	native->m_beastTypeName = "micay";
	native->m_beastTypeCRC = 0x8252A12E;
	EIBeast_LoadBeastAsset( native );

	EIBeast_Deserialize2( native );

	if ( native->m_beastType ) {
		log_printf( "Beast type asset load failed.\n" );
	}

	ERLevel_SetupEntity( harry->m_world, native, 0 );
	EINPCBeast_Init( native );

	AddToWorld( harry->m_world, native );
}


// --------------------------------------------------------------------------------
// List all loaded models.
// --------------------------------------------------------------------------------
static void _list_model_assets()
{
	TreeMapNode* node = ((TreeMap*)(0x91A9B8 + 0x14))->m_iterateFirst;
	while ( node ) {
		if ( node->m_ptr ) {
			EResource* res = (EResource*) node->m_ptr;
			log_printf( "- 0x%X : %s\n", res, res->m_name );
		}

		node = node->m_iterateNext;
	}
}


static void _list_updates_1()
{
	EIHarry* harry = *((EIHarry**) 0x917034);
	ERLevel* level = harry->m_world;

	TreeMapNode* node = ((TreeMap*)((uint32_t) level) + 0x5B0)->m_iterateFirst;

	while ( node ) {
		if ( node->m_ptr ) {
			EInstance* inst = (EInstance*) node->m_ptr;
			EIBeast* beast = type_cast<EIBeast>( (EStorable*) node->m_ptr, get_type_by_vtable(0x86C3D0) );

			const type_info_t* type = get_object_type( inst );

			if ( beast && beast->m_beastTypeName ) {
				log_printf( "- 0x%X : %s (%s)\n", beast, type ? type->get_name() : "UnknownType", beast->m_beastTypeName );
			}
			else {
				log_printf( "- 0x%X : %s\n", inst, type ? type->get_name() : "UnknownType" );
			}
		}
		else {
			log_printf( "- 0x??????? : unknown\n" );
		}
		node = node->m_iterateNext;
	}
}

// List entities in second update list.

static void _list_updates_2()
{
	EIHarry* harry = *((EIHarry**) 0x917034);
	ERLevel* level = harry->m_world;

	TreeMapNode* node = level->m_updateRegion.m_treeMap.m_iterateFirst;

	while ( node ) {
		if ( node->m_ptr ) {
			EInstance* inst = (EInstance*) node->m_ptr;
			EIBeast* beast = type_cast<EIBeast>( (EStorable*) node->m_ptr, get_type_by_vtable(0x86C3D0) );

			const type_info_t* type = get_object_type( inst );

			if ( beast && beast->m_beastTypeName ) {
				log_printf( "- 0x%X : %s (%s)\n", beast, type ? type->get_name() : "UnknownType", beast->m_beastTypeName );
			}
			else {
				log_printf( "- 0x%X : %s\n", inst, type ? type->get_name() : "UnknownType" );
			}
		}
		else {
			log_printf( "- 0x??????? : unknown\n" );
		}
		node = node->m_iterateNext;
	}
}



//
// List of available commands / cheats.
//

const command_t commands[] =
{
	/* Name,          On Enable */
	{ "Give Stuff",   _give_stuff },
	{ "Heal",         _heal },
	{ "",             _empty },
	{ "Die",          _die },
	{ "TNT Rain",     _tnt_rain },
	{ "All TNT",      _all_tnt },
	{ "Crack Native", _fast_native },
	{ "Spawn Idol",   _idol_spawn },
	{ "Spawn Micay",  _micay_spawn },
	{ "Spawn Native", _netiv_spawn },
	{ "",             _empty },
	{ "List models",  _list_model_assets },
	{ "List updates 1", _list_updates_1 },
	{ "List updates 2", _list_updates_2 },
};

const int NUM_COMMANDS = sizeof(commands) / sizeof(command_t);



//
// Command palette window.
//

#include <Windows.h>

static HWND hwndPalette;
static char className[] = "PaletteWindow";

static LRESULT CALLBACK PaletteWndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	int i = 0;

	switch ( msg )
	{
	case WM_CREATE:
		for ( const command_t* cmd = commands; cmd != commands + NUM_COMMANDS; cmd++, i++ ) {
			CreateWindow( "BUTTON", cmd->name,
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				10 + (i & 1) * 120, 10 + (i / 2) * 30,
				110, 20, hwnd, (HMENU) (i + 100), 0, 0 );
		}
		break;

	case WM_CLOSE:
		DestroyWindow( hwnd );
		return 0;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;

	case WM_COMMAND:
		{
			int id = wparam;

			if ( id >= 100 && id < 100 + NUM_COMMANDS ) {
				const command_t* cmd = &commands[id - 100];
				cmd->func();
			}
			else if ( id >= 400 && id < 400 + levels_game.count ) {
				ScheduleWorldLoad( (void*) 0x917028, levels_game.levels[id - 400].crc, false );
			}
			else if ( id >= 500 && id < 500 + levels_beta.count ) {
				ScheduleWorldLoad( (void*) 0x917028, levels_beta.levels[id - 500].crc, false );
			}
			else if ( id >= 600 && id < 608 ) {
				ItemStruct* items = (ItemStruct*) 0x8EEB90;
				items[id - 600].m_unlocked = !items[id - 600].m_unlocked;
			}
			else if ( id >= 608 && id < 614 ) {
				EIHarry* harry = *((EIHarry**) 0x917034);
				if ( harry ) {
					switch ( id ) {
					case 608: harry->m_risingStrike = !harry->m_risingStrike; break;
					case 609: harry->m_smashStrike  = !harry->m_smashStrike;  break;
					case 610: harry->m_heroicDash   = !harry->m_heroicDash;   break;
					case 611: harry->m_heroicDive   = !harry->m_heroicDive;   break;
					case 612: harry->m_superSling   = !harry->m_superSling;   break;
					case 613: harry->m_breakdance   = !harry->m_breakdance;   break;
					}
				}
			}
		}
		break;

	case WM_MENUSELECT:
		{
			if ( HIWORD( wparam ) & MF_POPUP ) {
				EIHarry* harry = *((EIHarry**) 0x917034);
				ItemStruct* items = (ItemStruct*) 0x8EEB90;
				HMENU subMenu = GetSubMenu( (HMENU) lparam, LOWORD(wparam) );

				switch ( LOWORD( wparam ) ) {
				case 1:
					for ( int i = 0; i < 8; i++ ) {
						CheckMenuItem( subMenu, 600 + i, (items[i].m_unlocked) ? MF_CHECKED : MF_UNCHECKED );
					}
					break;
				case 2:
					for ( int i = 0; i < 6; i++ ) {
						EnableMenuItem( subMenu, 608 + i, harry != 0 ? MF_ENABLED : MF_GRAYED );
					}
					CheckMenuItem( subMenu, 608, (harry != 0 && harry->m_risingStrike) ? MF_CHECKED : MF_UNCHECKED );
					CheckMenuItem( subMenu, 609, (harry != 0 && harry->m_smashStrike)  ? MF_CHECKED : MF_UNCHECKED );
					CheckMenuItem( subMenu, 610, (harry != 0 && harry->m_heroicDash)   ? MF_CHECKED : MF_UNCHECKED );
					CheckMenuItem( subMenu, 611, (harry != 0 && harry->m_heroicDive)   ? MF_CHECKED : MF_UNCHECKED );
					CheckMenuItem( subMenu, 612, (harry != 0 && harry->m_superSling)   ? MF_CHECKED : MF_UNCHECKED );
					CheckMenuItem( subMenu, 613, (harry != 0 && harry->m_breakdance)   ? MF_CHECKED : MF_UNCHECKED );
					break;
				}
			}
		}
		break;
	}

	return DefWindowProc( hwnd, msg, wparam, lparam );
}

static BOOL CALLBACK SetFontToAll( HWND hwnd, LPARAM lparam )
{
	SendMessage( hwnd, WM_SETFONT, lparam, 0 );
	return true;
}

static HMENU create_menu()
{
	HMENU menu = CreateMenu();

	HMENU goTo = CreateMenu(); {
		HMENU beta = CreateMenu(); {
			for ( int i = 0; i < levels_beta.count; i++ ) {
				AppendMenu( beta, MF_STRING, 500 + i, levels_beta.levels[i].name );
			}

			AppendMenu( goTo, MF_POPUP, (UINT_PTR) beta, "Beta" );
		}

		AppendMenu( goTo, MF_SEPARATOR, 0, 0 );

		for ( int i = 0; i < levels_game.count; i++ ) {
			AppendMenu( goTo, MF_STRING, 400 + i, levels_game.levels[i].name );
		}

		AppendMenu( menu, MF_POPUP, (UINT_PTR) goTo, "Go to" );
	}

	HMENU items = CreateMenu(); {
		AppendMenu( items, MF_STRING, 600, "Canteen" );
		AppendMenu( items, MF_STRING, 601, "Sling" );
		AppendMenu( items, MF_STRING, 602, "Torch" );
		AppendMenu( items, MF_STRING, 603, "Shield" );
		AppendMenu( items, MF_STRING, 604, "Gas Mask" );
		AppendMenu( items, MF_STRING, 605, "Raft" );
		AppendMenu( items, MF_STRING, 606, "Pickaxes" );
		AppendMenu( items, MF_STRING, 607, "TNT" );

		AppendMenu( menu, MF_POPUP, (UINT_PTR) items, "Items" );
	}

	HMENU skills = CreateMenu(); {
		AppendMenu( skills, MF_STRING, 608, "Rising Strike" );
		AppendMenu( skills, MF_STRING, 609, "Smash Strike" );
		AppendMenu( skills, MF_STRING, 610, "Heroic Dash" );
		AppendMenu( skills, MF_STRING, 611, "Heroic Dive" );
		AppendMenu( skills, MF_STRING, 612, "Super Sling" );
		AppendMenu( skills, MF_STRING, 613, "Breakdance" );

		AppendMenu( menu, MF_POPUP, (UINT_PTR) skills, "Skills" );
	}

	return menu;
}

void command_create_window()
{
	WNDCLASSEX wcex; ZeroMemory( &wcex, sizeof(wcex) );
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hInstance = GetModuleHandle(0);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground = (HBRUSH) COLOR_WINDOW;
	wcex.lpszClassName = className;
	wcex.lpfnWndProc = PaletteWndProc;

	RegisterClassEx( &wcex );

	RECT rect = { 600, 20, 600 + 250, 20 + 480 };
	DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

	AdjustWindowRect( &rect, style, false );

	HMENU menu = create_menu();
	hwndPalette = CreateWindow( className, "Cheat Palette", style,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		0, menu, GetModuleHandle(0), 0 );

	HFONT font = CreateFont( 16, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Segoe UI" );
	EnumChildWindows( hwndPalette, SetFontToAll, (LPARAM) font );

	ShowWindow( hwndPalette, SW_SHOW );
}

void command_process_window()
{
	MSG msg;
	while ( GetMessage(&msg, 0, 0, 0) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}
