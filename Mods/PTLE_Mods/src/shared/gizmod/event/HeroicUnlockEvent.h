#pragma once

#include "Event.h"

#include "ptle/EHeroicSkill.h"

class EInstance;


DECLARE_EVENT( HeroicUnlock );

class HeroicUnlockEvent : public Event<IHeroicUnlockListener>
{
public:

	HeroicUnlockEvent( EHeroicSkill::Enum skill )
		: m_skill( skill )
	{
	}

	inline EHeroicSkill::Enum getSkill() const { return m_skill; }


private:

	EHeroicSkill::Enum m_skill;
};
