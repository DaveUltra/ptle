#pragma once

#include "EItemBase.h"

#include "EAnimController.h"


class EItemRaft : public EItemBase
{
public:

	float unknown0;
	EAnimController m_animController;

	char padding0[0x4];

	bool m_inflated;

	char padding1[0x7];
};
