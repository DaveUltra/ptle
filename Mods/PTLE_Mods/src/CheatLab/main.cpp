#include <Windows.h>

#include "command_palette.h"
#include "utils/log.h"

#include "ptle/types/types.h"



unsigned long __stdcall Thread( void* );

void InitMod()
{
	AllocConsole();

	register_types();

	CreateThread( 0, 0, Thread, 0, 0, 0 );

	log_printf( "PTLE Cheat Lab : Mod started.\n" );
}

unsigned long __stdcall Thread( void* )
{
	command_create_window();
	command_process_window();

	ExitProcess( 0 );
}


BOOL WINAPI DllMain( HINSTANCE hinstace, DWORD reason, LPVOID )
{
	switch ( reason )
	{
	case DLL_PROCESS_ATTACH:
		InitMod();
		break;
	default:
		break;
	}

	return true;
}
