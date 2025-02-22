#pragma once

#include "EICharacter.h"

#include "math/Vector3f.h"


class EIPlayer : public EICharacter
{
public:

	Vector4f m_mat[3];
	Vector4f m_position;
	char padding4[0x304];

	int m_healthMax;
	int m_healthValue;

	char padding8[0x118];

	Vector3f m_headRotation;
	Vector3f m_headTargetRotation;

	char padding5[0x1D0];

	void** m_itemHotbar;

	char padding9[0x1F0];
};
