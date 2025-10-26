#include "gizmod/Gizmod.h"

#include "ptle/EHeroicSkill.h"
#include "ptle/EIHarry.h"


bool EHeroicSkill::isSkillUnlocked( Enum skill )
{
	EIHarry* harry = Gizmod::getHarry();
	if ( !harry ) return false;

	switch ( skill )
	{
	case RISING_STRIKE: return harry->m_risingStrike != 0;
	case SMASH_STRIKE:  return harry->m_smashStrike  != 0;
	case HEROIC_DASH:   return harry->m_heroicDash   != 0;
	case HEROIC_DIVE:   return harry->m_heroicDive   != 0;
	case BREAKDANCE:    return harry->m_breakdance   != 0;
	case SUPER_SLING:   return harry->m_superSling   != 0;
	}

	return false;
}

void EHeroicSkill::unlockSkill( Enum skill )
{
	EIHarry* harry = Gizmod::getHarry();
	if ( !harry ) return;

	switch ( skill )
	{
	case RISING_STRIKE: harry->m_risingStrike = 1; break;
	case SMASH_STRIKE:  harry->m_smashStrike  = 1; break;
	case HEROIC_DASH:   harry->m_heroicDash   = 1; break;
	case HEROIC_DIVE:   harry->m_heroicDive   = 1; break;
	case BREAKDANCE:    harry->m_breakdance   = 1; break;
	case SUPER_SLING:   harry->m_superSling   = 1; break;
	}
}
