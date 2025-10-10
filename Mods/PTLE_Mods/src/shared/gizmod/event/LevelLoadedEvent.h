#pragma once

#include "Event.h"


DECLARE_EVENT( LevelLoaded );

class LevelLoadedEvent : public Event<ILevelLoadedListener>
{
public:

	LevelLoadedEvent() { }

	inline void setMusicOverride( uint32_t musicCRC ) { m_musicCRC = musicCRC; }

	inline uint32_t getMusicCRC() const { return m_musicCRC; }


private:

	uint32_t m_musicCRC;
};
