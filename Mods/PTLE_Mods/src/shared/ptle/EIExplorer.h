#pragma once

#include "EIBeast.h"


class EIExplorer : public EIBeast
{
public:

	char padding4[12];

	bool m_explorerBool0;
	bool m_explorerBool1;
	bool m_explorerBool2;

	char padding[0x71];
};
