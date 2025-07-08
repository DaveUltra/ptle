#pragma once

#include <Windows.h>


class Logger
{
public:

	Logger();

	void log( const char* str );
	void log_printf( const char* fmt, ... );


private:

	void log_prefix();
	void log_internal( const char* str, int len );

private:

	HANDLE m_stdout;
	bool m_newLine;
};
