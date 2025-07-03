#include "PitfallPlugin.h"

#include <Windows.h>

#include "injector/injector.hpp"
#include "utils/log.h"


static void check_for_exit()
{
	HWND hwnd = *((HWND*) 0x93ED60);
	if ( !IsWindowVisible( hwnd ) ) {
		ExitProcess( 0 );
	}
}

static void adjust_window_size()
{
	int* width = (int*) 0x940D98;
	int* height = (int*) 0x940D9C;

	RECT rect = { 0, 0, *width, *height };
	AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX), false );

	*width = rect.right - rect.left;
	*height = rect.bottom - rect.top;
}
#include <d3d9.h>
void InitMod()
{
	// System inconveniences.
	{
		// Remove explorer.exe locking.
		injector::MakeRangedNOP( 0x60DFCB, 0x60DFDA );
		injector::MakeRangedNOP( 0x60E022, 0x60E03A );

		// Remove ShowCursor(0).
		injector::MakeRangedNOP( 0x60D6BA, 0x60D6BA+6 );

		// Window styles.
		injector::WriteMemory<uint32_t>( 0x6E860A, WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX) );
	}

	// Functional window.
	{
		// Process window messages (game executes its main loop in a new thread which didn't create the window...)
		injector::MakeCALL( 0x68F231, 0x6E83B0 );
		injector::MakeJMP( 0x68F236, 0x68EC2A );

		// Game messes with usual window messages here. Redirect to DefWindowProc().
		injector::MakeJMP( 0x60CFFA, 0x60D1C5 );

		// Check whether window was closed (for some reason WM_CLOSE is never received).
		injector::MakeRangedNOP( 0x68EC39, 0x68ECDA );
		injector::MakeCALL( 0x68EC39, check_for_exit );
	}

	// Renderer-related corrections.
	{
		// Allow Direct3D 9 context to be windowed, instead of fullscreen.
		injector::WriteMemory<uint32_t>( 0x697EF4, 0xEF4 );  // Replace "d3dPresentParams.RefreshRateInHz = 0"
		injector::WriteMemory<uint32_t>( 0x697EF8, 1 );      // by      "d3dPresentParams.WindowedMode = 1"

		// Correct window resolution.
		injector::MakeCALL( 0x60D4CD, adjust_window_size );  // We replace a "ShowCursor()" call by a function
		injector::MakeNOP( 0x60D4CD+5 );                     // that corrects client size based on window decorations.

		// Remove the game's adjust and move our own values.
		injector::MakeRangedNOP( 0x6E859C, 0x6E85E6 );
		char bytes[] = { 0xA1, 0x98, 0x0D, 0x94, 0x00, 0x8B, 0x3D, 0x9C, 0x0D, 0x94, 0x00 };  // mov eax, [940D98]; mov edi, [940D9C]
		injector::WriteMemoryRaw( 0x6E859C, bytes, sizeof(bytes), true );
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


class WindowedModePlugin : public PitfallPlugin
{
public:

	virtual const char* getDisplayName() const { return "PTLE Windowed Mode"; }
	virtual const char* getSystemName() const { return "WindowedMode"; }

	virtual void onEnable()
	{
		InitMod();
	}
};

DECLARE_PLUGIN( WindowedModePlugin );
