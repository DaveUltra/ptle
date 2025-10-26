inline bool EHarryActions::isActionEnabled( Enum action )
{
	uint32_t* bitfield = ((uint32_t*) 0x90FC10);
	return ((*bitfield) & action) == 0;
}

inline void EHarryActions::enableAction( Enum action )
{
	uint32_t* bitfield = ((uint32_t*) 0x90FC10);
	*bitfield = *bitfield & ~action;
}

inline void EHarryActions::disableAction( Enum action )
{
	uint32_t* bitfield = ((uint32_t*) 0x90FC10);
	*bitfield = *bitfield | action;
}

inline void EHarryActions::enableAll()
{
	uint32_t* bitfield = ((uint32_t*) 0x90FC10);
	*bitfield = 0;
}

inline void EHarryActions::disableAll()
{
	uint32_t* bitfield = ((uint32_t*) 0x90FC10);
	*bitfield = 0xFFFFFFFF;
}


inline uint32_t EHarryActions::getActionsBitfield()
{
	return *((uint32_t*) 0x90FC10);
}

inline void EHarryActions::setActionsBitfield( uint32_t v )
{
	*((uint32_t*) 0x90FC10) = v;
}
