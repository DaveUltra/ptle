#pragma once

#include "EIBeast.h"

#include "ptle/containers/Array/Array.h"


class EIHenchman : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x881BE8;


	bool m_hasSpotlight;

	uint32_t m_spotlightModelHash;
	uint32_t m_projectileType;
	float unknown4;
	uint32_t unknown5;

	char padding7[0x1C];

	Array m_coverObjectHashes;  // List of object IDs this henchman can hide behind.

	char padding8[0x48];

	uint32_t unknown6;
	uint32_t m_boneFinger1_L;
	uint32_t m_boneFinger1_R;
	uint32_t m_boneHand_L;
	uint32_t unknown7;

	char padding9[0x08];

	bool m_pointReached;
	bool unused0;
	bool m_coverObjectDestroyed;

	int m_coverObjectID;
	int m_desiredCoverObjectID;
	int m_closestJumpPoint;
	Vector3f m_jumpPointLocation;

	char padding10[0x10];

	bool m_standing;

	char padding11[0xF];

	Array m_coverObjectInstances;

	EIDynamicModel* m_spotlightModel;

	char padding12[0x04];
};
