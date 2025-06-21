#pragma once

#include "EIBeast.h"

#include "ptle/math/Matrix4f.h"
#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"

class EIMegaMonkeyParent;


class EIEvilHarryMM : public EIBeast
{
public:

	int m_bossStage;
	int unknown2;
	uint32_t unknown3;

	Matrix4f unknown4;
	Matrix4f unknown5;

	uint32_t unknown6;

	Matrix4f unknown7;
	Matrix4f unknown8;

	uint32_t unknown9;

	Matrix4f unknown10;

	bool unknown11;

	int unknown12;

	uint32_t m_mamaMonkeyID;

	Vector3f unknown13;
	Vector4f unknown14;

	char padding8[0x0C];

	int unknown15[4];

	EIMegaMonkeyParent* m_mamaMonkey;

	char padding9[0x08];
};
