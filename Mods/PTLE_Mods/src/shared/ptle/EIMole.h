#pragma once

#include "EIBeast.h"

class EIProjectile;


class EIMole : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x88A668;


	int unknown0;

	uint32_t m_boneHand_L;
	char* m_projectileTypePath;

	char padding[0x4];

	uint32_t m_projectileType;
	EIProjectile* m_projectile;
	bool m_threwTNT;
};
