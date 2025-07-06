#pragma once

#include "Event.h"

#include <stdint.h>


DECLARE_EVENT( LoadLevel );

class LoadLevelEvent : public Event<ILoadLevelListener>
{
public:

	LoadLevelEvent( uint32_t levelCRC );

private:


};
