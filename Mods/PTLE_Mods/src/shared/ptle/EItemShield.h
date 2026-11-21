#pragma once

#include "EItemBase.h"

#include "EAnimController.h"

class EIDynamicModel;


class EItemShield : public EItemBase
{
public:

	float unknown0;
	EAnimController m_animController;

	char padding0[0x8];

	EIDynamicModel* m_havokPhantom;

	Matrix4f unknown1;

	char padding1[0x10];
};
