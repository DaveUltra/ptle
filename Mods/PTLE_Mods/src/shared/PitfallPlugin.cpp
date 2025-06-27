#include "PitfallPlugin.h"

#include <vector>


static std::vector<ILoadLevelEvent*> g_loadLevelCallbacks;



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
