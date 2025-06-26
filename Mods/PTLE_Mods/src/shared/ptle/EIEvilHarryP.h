#pragma once

#include "EIBeast.h"

#include "ptle/math/Matrix4f.h"
#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"

class EIDynamicModel;


class EIEvilHarryP : public EIBeast
{
public:

	char padding8[0x04];

	int m_bossPhase;

	char padding9[0x14];

	Vector3f unknown2;
	Vector3f unknown3;

	char padding10[0x0C];

	Matrix4f unknown4;
	float unknown5;
	float unknown6;

	char padding11[0x10];

	uint32_t m_boneHand_L;
	Matrix4f unknown7;
	Matrix4f unknown8;
	uint32_t m_boneHand_R;
	Matrix4f unknown9;
	Matrix4f unknown10;
	uint32_t m_boneToe_L;
	Matrix4f unknown11;
	Matrix4f unknown12;

	char padding12[0x20];

	Vector3f unknown13;
	EICharacter* m_playerIceCube;
	EIDynamicModel* m_pickaxeModel0;
	EIDynamicModel* m_pickaxeModel1;

	char padding13[0x08];
};
