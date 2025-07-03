#include "Pitfall.h"

#include "PitfallPlugin.h"


extern "C" { extern Pitfall* pitfallInstance = 0; }


Pitfall::Pitfall()
{

}

/*static*/ Pitfall* Pitfall::getInstance()
{
	return pitfallInstance;
}

PitfallPlugin* Pitfall::getThisPlugin()
{
	return PitfallPlugin::getInstance();
}
