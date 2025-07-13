#pragma once

#include "Event.h"


DECLARE_EVENT( LevelLoaded );

class LevelLoadedEvent : public Event<ILevelLoadedListener>
{
public:

	LevelLoadedEvent() { }
};
