#pragma once

#include "EIBeast.h"


class EIPusca : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x893EA8;


	Vector3f unknown0;

	uint32_t m_boneHandCenterR;
	Matrix4f unknown1;

	char padding0[0x08];

	uint32_t m_projectileTypeFireball;
	uint32_t m_projectileType2;
	uint32_t m_projectileHeatSeeker;
	EIProjectile* m_projectile;
	EIGameInstance* m_projectileGen;
	bool m_firedProjectile;

	char padding1[0x0B];

	Vector3f unknown2;

	int m_bossStage;
	int m_numTimesHit;

	float unknown3;
	float m_pillarOfDeathTimer;

	int unknown4;

	bool m_wasHarryHit;

	EIParticleEmit* unknown5[6];

	char padding2[0x18];

	uint32_t m_someStaticModelCRC;
	EIStaticModel* m_someStaticModel;

	char padding3[0x08];

	Vector3f m_savedPositionHidden;
	bool m_hiddenWhileMinionsKill;
};
