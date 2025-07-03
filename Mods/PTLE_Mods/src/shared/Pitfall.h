#pragma once

#include "PitfallEvents.h"

class PitfallPlugin;


class Pitfall
{
public:

	Pitfall();

	static Pitfall* getInstance();

	static PitfallPlugin* getThisPlugin();

	template<class T>
	void registerEvent(  );


private:

	PitfallEvents m_events;
};
