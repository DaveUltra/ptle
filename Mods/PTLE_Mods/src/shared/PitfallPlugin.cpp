#include "PitfallPlugin.h"

#include <vector>


static std::vector<ILoadLevelEvent*> g_loadLevelCallbacks;

PitfallPlugin* PitfallPlugin::m_instance = 0;



PitfallPlugin::PitfallPlugin()
	: m_loggerNewLine( true )
{
	m_instance = this;
}

std::wstring PitfallPlugin::getConfigDirectory() const
{
	std::string cname = this->getSystemName();
	std::wstring name( cname.begin(), cname.end() );

	return L"config/" + name + L'/';
}

void PitfallPlugin::log_printf( const char* fmt, ... )
{
	va_list args;
	va_start( args, fmt );

	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	char msgBuf[1024];
	char* msgPtr;
	int len;

	if ( m_loggerNewLine ) {
		log_prefix();
	}
	len = vsprintf_s( msgBuf, fmt, args );
	msgPtr = msgBuf;

	while ( len > 0 ) {
		if ( m_loggerNewLine ) {
			log_prefix();
		}

		char* n = strchr( msgPtr, '\n' );
		if ( n ) {
			m_loggerNewLine = true;
		}
		else {
			n = msgPtr + len;
		}
		WriteConsoleA( stdOut, msgPtr, n - msgPtr + 1, 0, 0 );
		len -= (n - msgPtr + 1);
		msgPtr = n + 1;
	}

	va_end( args );
}

void PitfallPlugin::log_prefix()
{
	char msgBuf[128];

	m_loggerNewLine = false;
	int len = sprintf_s( msgBuf, "[%s] ", this->getDisplayName() );
	WriteConsoleA( GetStdHandle(STD_OUTPUT_HANDLE), msgBuf, len, 0, 0 );
}


EIHarry* PitfallPlugin::getHarry()
{
	return *((EIHarry**) 0x917034);
}

ERLevel* PitfallPlugin::getLevel()
{
	return **((ERLevel***) 0x91702C);
}

uint32_t PitfallPlugin::getCurrentLevelCRC()
{
	return *((uint32_t*) 0x917088);
}



void PitfallPlugin::registerLoadLevel( ILoadLevelEvent* e )
{
	g_loadLevelCallbacks.push_back( e );
}
