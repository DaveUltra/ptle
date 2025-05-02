#pragma once

#include "EIBeast.h"

class EIProjectile;


class EINative : public EIBeast
{
public:

	char padding5[0x18];

	uint32_t m_projectileType;
	EIProjectile* m_projectile;

	char padding6[0x30];
};
