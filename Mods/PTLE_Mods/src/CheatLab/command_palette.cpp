#include "command_palette.h"

#include "injector/injector.hpp"

#include "ptle/EIHarry.h"
#include "ptle/EIProjectile.h"
#include "ptle/math/Vector3f.h"

#include "ptle/levels/level_info.h"

#include "utils/log.h"


// Create a function pointer to a known non-member function.
#define GET_FUNC( addr, rettype, name, ... ) \
	static rettype (*name)(__VA_ARGS__) = reinterpret_cast<rettype(*)(__VA_ARGS__)>(addr)

// Create a function pointer to a known method (member function).
#define GET_METHOD( addr, rettype, name, ... ) \
	static rettype (__thiscall *name)(__VA_ARGS__) = reinterpret_cast<rettype(__thiscall *)(__VA_ARGS__)>(addr)

// Make a custom function compatible with the __thiscall convention.
#define MAKE_THISCALL_WRAPPER( funcName, funcPtr ) \
	static __declspec(naked) void funcName() { \
		__asm pop edi \
		__asm push ecx \
		__asm push edi \
		__asm jmp dword ptr funcPtr \
	}



GET_METHOD( 0x5EBA90, void, ScheduleWorldLoad, void*, uint32_t, bool );

GET_FUNC( 0x557840, EIProjectile*, EIProjectile_Create, uint32_t );
GET_METHOD( 0x559640, void, EIProjectile_Spawn, EIProjectile*, Vector3f*, Vector3f*, EInstance*, float );



static void _empty()
{

}

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
}

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

static void _die()
{
	EIHarry* harry = injector::ReadMemory<EIHarry*>( 0x917034 );
	if ( !harry ) return;

	harry->m_healthValue = 0;
}

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

static void _all_tnt()
{
	// mov ecx, BBCAC492
	uint8_t bytes[] = { 0xC7, 0x45, 0x08, 0x92, 0xC4, 0xCA, 0xBB };
	injector::WriteMemoryRaw( 0x55784E, bytes, sizeof(bytes), true );
}

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
