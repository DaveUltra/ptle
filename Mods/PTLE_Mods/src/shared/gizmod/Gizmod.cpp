#include "Gizmod.h"

#include "GizmodPlugin.h"

#include <Windows.h>


extern "C" { extern Gizmod* gizmodInstance; }


Gizmod::Gizmod()
{

}

/*static*/ Gizmod* Gizmod::getInstance()
{
	return gizmodInstance;
}

GizmodPlugin* Gizmod::getThisPlugin()
{
	return GizmodPlugin::getInstance();
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
