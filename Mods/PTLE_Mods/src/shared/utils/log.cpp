#include "log.h"

#include <Windows.h>
#include <stdio.h>


void log_printf( const char* fmt, ... )
{
	va_list args;
	va_start( args, fmt );

	char msg[512];
	int len = vsprintf_s( msg, fmt, args );
	WriteConsole( GetStdHandle(STD_OUTPUT_HANDLE), msg, len, 0, 0 );

	va_end( args );
}
