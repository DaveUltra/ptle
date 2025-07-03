#include "Pitfall.h"

#include "PitfallPlugin.h"


extern "C" { __declspec(dllexport) Pitfall* pitfallInstance = 0; }


Pitfall::Pitfall()
{

}

PitfallPlugin* Pitfall::getThisPlugin()
{
	return PitfallPlugin::getInstance();
}
