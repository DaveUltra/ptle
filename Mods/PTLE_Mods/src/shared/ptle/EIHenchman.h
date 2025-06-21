#pragma once

#include "EIBeast.h"

#include "ptle/containers/Array/Array.h"


class EIHenchman : public EIBeast
{
public:

	bool unknown2;
	bool unknown3;

	uint32_t m_modelHash;   // Probably for handheld TNT model.
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

	char padding9[0x44];

	Array m_coverObjectInstances;

	EIStaticModel* m_model;

	char padding10[0x04];
};
