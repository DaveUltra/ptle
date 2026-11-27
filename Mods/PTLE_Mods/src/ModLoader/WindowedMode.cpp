#include <Windows.h>
#include <d3d9.h>

#include "injector/injector.hpp"
#include "utils/func.h"


static void check_for_exit()
{
	HWND hwnd = *((HWND*) 0x93ED60);
	if ( !IsWindowVisible(hwnd) ) {
		ExitProcess( 0 );
	}
}

static void adjust_window_size()
{
	int* width = (int*) 0x940D98;
	int* height = (int*) 0x940D9C;

	RECT rect = { 0, 0, *width, *height };
	AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, false );

	*width = rect.right - rect.left;
	*height = rect.bottom - rect.top;
}


// Resizing : the game keeps drawing at its own resolution, and Present() scales that picture to the
// window, keeping its shape (centered, black borders). Resetting the device to the window's size isn't
// safe while the game runs (its threads crash in d3d9 or deadlock).
static IDirect3DDevice9** const     g_d3dDevice        = (IDirect3DDevice9**)     0x941080;
static D3DPRESENT_PARAMETERS* const g_d3dPresentParams = (D3DPRESENT_PARAMETERS*) (0x926790 + 0xED4);

static RECT get_picture_rect()
{
	RECT client;
	GetClientRect( *((HWND*) 0x93ED60), &client );
	const int w = client.right, h = client.bottom;
	const int bw = g_d3dPresentParams->BackBufferWidth, bh = g_d3dPresentParams->BackBufferHeight;

	int pw = w, ph = h;
	if ( bw > 0 && bh > 0 ) {
		if ( w * bh > h * bw ) pw = h * bw / bh;
		else                   ph = w * bh / bw;
	}
	if ( pw < 1 ) pw = 1;
	if ( ph < 1 ) ph = 1;

	RECT r;
	r.left = (w - pw) / 2;
	r.top = (h - ph) / 2;
	r.right = r.left + pw;
	r.bottom = r.top + ph;
	return r;
}

// Present() only takes a destination rectangle with the "copy" swap effect. With "discard", it fails,
// and the game takes that as a lost device and stops drawing.
static HRESULT present_picture()
{
	RECT client, picture = get_picture_rect();
	GetClientRect( *((HWND*) 0x93ED60), &client );
	const bool centered = g_d3dPresentParams->SwapEffect == D3DSWAPEFFECT_COPY && !EqualRect( &client, &picture );
	return (*g_d3dDevice)->Present( NULL, centered ? &picture : NULL, NULL, NULL );
}

// Multisampling (byte 0x940D90) needs "discard" : the picture then fills the window.
static void __declspec(naked) set_multisampling()
{
	__asm mov dword ptr [edx + 0xEE4], D3DMULTISAMPLE_2_SAMPLES     // Replaced instruction.
	__asm mov dword ptr [edx + 0xEEC], D3DSWAPEFFECT_DISCARD
	__asm ret
}

void PaintWindowBorders( HDC dc )
{
	RECT client, picture = get_picture_rect();
	GetClientRect( *((HWND*) 0x93ED60), &client );
	HBRUSH black = (HBRUSH) GetStockObject( BLACK_BRUSH );

	RECT r;
	SetRect( &r, 0, 0, client.right, picture.top );                          FillRect( dc, &r, black );
	SetRect( &r, 0, picture.bottom, client.right, client.bottom );           FillRect( dc, &r, black );
	SetRect( &r, 0, picture.top, picture.left, picture.bottom );             FillRect( dc, &r, black );
	SetRect( &r, picture.right, picture.top, client.right, picture.bottom ); FillRect( dc, &r, black );
}


GET_FUNC( 0x6E83B0, void, PeekWindowMessages );

void PatchWindowedModeAndAnnoyances()
{
	// System inconveniences.
	{
		// Remove explorer.exe locking.
		injector::MakeRangedNOP( 0x60DFCB, 0x60DFDA );
		injector::MakeRangedNOP( 0x60E022, 0x60E03A );

		// Window styles.
		injector::WriteMemory<uint32_t>( 0x6E860A, WS_OVERLAPPEDWINDOW );
	}

	// Functional window.
	{
		// Process window messages (game executes its main loop in a new thread which didn't create the window...)
		injector::MakeCALL( 0x68F231, PeekWindowMessages );

		// Skip forcing focus & activation.
		injector::MakeJMP( 0x68F236, 0x68EC2A );

		// Game messes with usual window messages here. Redirect to DefWindowProc().
		injector::MakeJMP( 0x60CFFA, 0x60D1C5 );
	}

	// Renderer-related corrections.
	{
		// Allow Direct3D 9 context to be windowed, instead of fullscreen.
		injector::WriteMemory<uint32_t>( 0x697EF4, 0xEF4 );  // Replace "d3dPresentParams.RefreshRateInHz = 0"
		injector::WriteMemory<uint32_t>( 0x697EF8, 1 );      // by      "d3dPresentParams.WindowedMode = 1"

		// Correct window resolution (Win32's AdjustWindowRect(), which ensures proper client rect resolution).
		injector::MakeCALL( 0x60D4CD, adjust_window_size );  // Replaces a superfluous
		injector::MakeNOP( 0x60D4CD+5 );                     // "ShowCursor()" call.

		// Remove the game's adjust and move our own values.
		injector::MakeRangedNOP( 0x6E859C, 0x6E85E6 );
		char bytes[] = { 0xA1, 0x98, 0x0D, 0x94, 0x00, 0x8B, 0x3D, 0x9C, 0x0D, 0x94, 0x00 };  // mov eax, [940D98]; mov edi, [940D9C]
		injector::WriteMemoryRaw( 0x6E859C, bytes, sizeof(bytes), true );

		// "Copy" swap effect, with a single back buffer (for resizing, see present_picture()).
		injector::WriteMemory<uint32_t>( 0x697E74, 1 );
		injector::WriteMemory<uint32_t>( 0x697EAE, D3DSWAPEFFECT_COPY );
		injector::MakeNOP( 0x697F6E, 10 );
		injector::MakeCALL( 0x697F6E, set_multisampling );

		// The picture in the middle of the window (the game's three Present() calls, from the pushes to the call).
		injector::MakeRangedNOP( 0x6A2427, 0x6A2441 );
		injector::MakeCALL( 0x6A2427, present_picture );
		injector::MakeRangedNOP( 0x69A002, 0x69A01B );   // These two store the result in [ebp-4] next, as before.
		injector::MakeCALL( 0x69A002, present_picture );
		injector::MakeRangedNOP( 0x69A05C, 0x69A076 );
		injector::MakeCALL( 0x69A05C, present_picture );
	}

	/*injector::MakeRangedNOP( 0x669C87, 0x669CA9 );
	injector::MakeNOP( 0x669C27, 8 );
	injector::MakeNOP( 0x669C37, 8 );
	injector::MakeNOP( 0x669C4E, 8 );
	injector::MakeNOP( 0x669C60, 8 );
	injector::MakeNOP( 0x669CA9, 8 );

	injector::MakeNOP( 0x699C7E, 8 );
	injector::MakeNOP( 0x699C9E, 8 );

	injector::MakeNOP( 0x6944DF, 8 );

	// MainGameLoop
	injector::MakeNOP( 0x68A09A, 8 );
	injector::MakeNOP( 0x68A126, 8 );
	injector::MakeNOP( 0x68A13E, 8 );

	// FindAsset blockers.
	injector::MakeRangedNOP( 0x620C0C, 0x620C19 );
	injector::MakeRangedNOP( 0x620C6F, 0x620C72 );
	injector::MakeRangedNOP( 0x620C51, 0x620C54 );*/
}
