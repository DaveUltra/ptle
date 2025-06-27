#pragma once

#include "Event.h"

#include <stdint.h>


class LoadLevelEvent : public Event
{
public:

	LoadLevelEvent( uint32_t levelCRC );

private:


};


class ILoadLevelListener
{
public:

	virtual void onLoadLevel( LoadLevelEvent& e ) = 0;
};
