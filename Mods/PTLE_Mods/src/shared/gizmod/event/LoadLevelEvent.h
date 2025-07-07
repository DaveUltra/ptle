#pragma once

#include "Event.h"

#include <stdint.h>


DECLARE_EVENT( LoadLevel );

class LoadLevelEvent : public Event<ILoadLevelListener>
{
public:

	LoadLevelEvent( uint32_t levelCRC, uint32_t prevCRC )
		: m_levelCRC( levelCRC ), m_prevCRC( prevCRC )
	{
	}

	inline uint32_t getLevelCRC() const { return m_levelCRC; }
	inline uint32_t getPrevCRC() const { return m_prevCRC; }

	inline void setLevelCRC( uint32_t crc ) { m_levelCRC = crc; }
	inline void setPrevCRC( uint32_t crc ) { m_prevCRC = crc; }


private:

	uint32_t m_levelCRC;
	uint32_t m_prevCRC;
};
