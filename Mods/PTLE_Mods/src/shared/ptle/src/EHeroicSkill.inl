inline EHeroicSkill::Enum EHeroicSkill::getSkillFromHash( uint32_t hash )
{
	switch ( hash )
	{
	case 0xA6A824C5: return RISING_STRIKE;
	case 0xCF35F353: return SMASH_STRIKE;
	case 0x987E5498: return HEROIC_DASH;
	case 0xC8ADF5BE: return HEROIC_DIVE;
	case 0x6B0E0329: return BREAKDANCE;
	case 0x073B8EE6: return SUPER_SLING;
	default: return UNKNOWN;
	}
}

inline uint32_t EHeroicSkill::getSkillHash( Enum skill )
{
	switch ( skill )
	{
	case RISING_STRIKE: return 0xA6A824C5;
	case SMASH_STRIKE:  return 0xCF35F353;
	case HEROIC_DASH:   return 0x987E5498;
	case HEROIC_DIVE:   return 0xC8ADF5BE;
	case BREAKDANCE:    return 0x6B0E0329;
	case SUPER_SLING:   return 0x073B8EE6;
	default: return 0;
	}
}
