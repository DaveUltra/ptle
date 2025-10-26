#pragma once

#include <stdint.h>


struct EHarryActions {
	enum Enum
	{
		CROUCH = 0x01,
		LOOK   = 0x02,
		SNEAK  = 0x04,
		ITEMS  = 0x08,
		DASH   = 0x10,
		ATTACK = 0x20,
		JUMP   = 0x40,
	};

public:

	static inline bool isActionEnabled( Enum action );

	static inline void enableAction( Enum action );
	static inline void disableAction( Enum action );

	static inline void enableAll();
	static inline void disableAll();


	static inline uint32_t getActionsBitfield();
	static inline void setActionsBitfield( uint32_t v );
};


#include "ptle/src/EHarryAction.inl"
