#include "Gizmod.h"

#include "GizmodPlugin.h"

#include <Windows.h>

#include <sstream>


extern "C" { extern Gizmod* gizmodInstance; }


/*static*/ int Gizmod::checkVersion( GizmodPlugin* p )
{
	if ( p->m_versionMajor > VERSION_MAJOR || p->m_versionMinor > VERSION_MINOR || p->m_versionPatch > VERSION_PATCH ) {
		return 2;
	}
	if ( p->m_versionMajor < VERSION_MAJOR || p->m_versionMinor < VERSION_MINOR ) {
		return -2;
	}
	if ( p->m_versionPatch < VERSION_PATCH ) {
		return -1;
	}
	return 0;
}


Gizmod::Gizmod()
{

}

/*static*/ Gizmod* Gizmod::getInstance()
{
	return gizmodInstance;
}

/*static*/ GizmodPlugin* Gizmod::getThisPlugin()
{
	return GizmodPlugin::getInstance();
}


std::string Gizmod::getVersionString() const
{
	std::stringstream ss;
	ss << VERSION_MAJOR << '.' << VERSION_MINOR << '.' << VERSION_PATCH;
	return ss.str();
}


void Gizmod::showConsole()
{
	AllocConsole();
}


EIHarry* Gizmod::getHarry()
{
	return *((EIHarry**) 0x917034);
}

ERLevel* Gizmod::getLevel()
{
	return **((ERLevel***) 0x91702C);
}

uint32_t Gizmod::getCurrentLevelCRC()
{
	return *((uint32_t*) 0x917088);
}
