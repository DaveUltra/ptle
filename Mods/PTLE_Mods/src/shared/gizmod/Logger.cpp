#include "Logger.h"

#include "GizmodPlugin.h"

#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>


Logger::Logger()
	: m_newLine( true )
{
	
}

void Logger::log( const char* str )
{
	this->log_internal( str, strlen(str) );
}

void Logger::log_printf( const char* fmt, ... )
{
	va_list args;
	va_start( args, fmt );

	char msgBuf[1024];
	int len;
	len = vsprintf_s( msgBuf, fmt, args );

	this->log_internal( msgBuf, len );

	va_end( args );
}


void Logger::log_prefix()
{
	char msgBuf[128];

	m_stdout = GetStdHandle( STD_OUTPUT_HANDLE );

	m_newLine = false;
	int len = sprintf_s( msgBuf, "[%s] ", GizmodPlugin::getInstance()->getDisplayName() );
	WriteConsoleA( m_stdout, msgBuf, len, 0, 0 );
}

void Logger::log_internal( const char* str, int len )
{
	const char* msgPtr;

	m_stdout = GetStdHandle( STD_OUTPUT_HANDLE );

	msgPtr = str;

	while ( len > 0 ) {
		if ( m_newLine ) {
			log_prefix();
		}

		const char* n = strchr( msgPtr, '\n' );
		if ( n ) {
			m_newLine = true;
		}
		else {
			n = msgPtr + len;
		}
		WriteConsoleA( m_stdout, msgPtr, n - msgPtr + 1, 0, 0 );
		len -= (n - msgPtr + 1);
		msgPtr = n + 1;
	}
}
