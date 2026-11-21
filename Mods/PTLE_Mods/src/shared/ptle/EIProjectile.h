#pragma once

#include "EIDynamicModel.h"

class EResource;


struct ProjectileTypeInfo
{
	uint32_t m_typeID;
	/* ... */
};

class EIProjectile : public EIDynamicModel
{
public:

	static const uint32_t VTABLE = 0x893210;


	float unknown0;
	float m_hitDamage;
	float m_hitForce;
	float m_lifetime;
	bool unknown2;
	bool unknown3;

	EResource* unknown4;
	ProjectileTypeInfo* m_projectileTypeInfo;

	uint32_t unknown13;
	uint32_t unknown14;
	uint32_t m_hitFlags;

	float unknown6;
	float unknown7;
	Vector3f unknown8;

	char padding5[0x04];

	Vector3f unknown9;
	Vector3f m_direction;
	Vector3f unknown11;
	Vector3f unknown12;

	EICharacter* m_source;
	void* /*EParticleRibbon*/ m_particleRibbon;
	void* m_particleRibbonData;

	char padding6[0x24];
};
