#pragma once

#include <stdint.h>


struct EHeroicSkill
{
	enum Enum
	{
		RISING_STRIKE,
		SMASH_STRIKE,
		HEROIC_DASH,
		HEROIC_DIVE,
		BREAKDANCE,
		SUPER_SLING,

		NUM_SKILLS,
		UNKNOWN = -1
	};

	int      m_unlocked;
	uint32_t m_hashID;

public:

	static inline Enum getSkillFromHash( uint32_t hash );

	static inline uint32_t getSkillHash( Enum skill );

		/// TODO : Current implementation relies on Harry being spawned, meaning this function will
		/// return false in between loads. Please fix by probing the save game manager instead.
	static bool isSkillUnlocked( Enum skill );

	static void unlockSkill( Enum skill );
};


#include "ptle/src/EHeroicSkill.inl"
