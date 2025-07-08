#pragma once

#include "EIDynamicModel.h"

class EResource;


class EIProjectile : public EIDynamicModel
{
public:

	float unknown0;
	float unknown0_1;
	float unknown0_2;
	float m_lifetime;
	bool unknown2;
	bool unknown3;

	EResource* unknown4;
	void* m_projectileTypeInfo;

	Vector3f unknown5;
	float unknown6;
	float unknown7;
	Vector3f unknown8;

	char padding5[0x04];

	Vector3f unknown9;
	Vector3f unknown10;
	Vector3f unknown11;
	Vector3f unknown12;

	EICharacter* m_source;
	void* /*EParticleRibbon*/ m_particleRibbon;
	void* m_particleRibbonData;

	char padding6[0x24];
};
