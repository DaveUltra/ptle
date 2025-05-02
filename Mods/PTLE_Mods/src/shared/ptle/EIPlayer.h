#pragma once

#include "EICharacter.h"

#include "math/Vector3f.h"


class EIPlayer : public EICharacter
{
public:

	// TODO : replace by a matrix4.
	Vector4f m_mat[3];
	Vector4f m_positionPlayer;

	char padding4[0x2E0];

	Vector3f unknown1;
	Vector3f unknown2;
	Vector3f unknown3;

	int m_healthMax;
	int m_healthValue;

	char padding8[0x118];

	Vector3f m_headRotation;
	Vector3f m_headTargetRotation;

	char padding5[0xD4];

	Matrix4f m_rigidBodyMatrix;

	char padding6[0xBC];

	void** m_itemHotbar;

	char padding9[0x1F0];
};
