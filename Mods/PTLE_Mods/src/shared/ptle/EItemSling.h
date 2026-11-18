#pragma once

#include "EItemBase.h"

#include "EAnimController.h"


class EItemSling : public EItemBase
{
public:

	EAnimController m_animController;

	char padding[0x40];
};
