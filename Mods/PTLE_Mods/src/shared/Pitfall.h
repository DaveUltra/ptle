#pragma once

#include "PitfallEvents.h"

class PitfallPlugin;


class Pitfall
{
public:

	Pitfall();

	static Pitfall* getInstance();

	static PitfallPlugin* getThisPlugin();

	inline PitfallEvents* getEventListener() { return &m_events; }


private:

	PitfallEvents m_events;
};
