#pragma once

#include "EIBeast.h"


class EIExplorer : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x876400;


	char padding4[12];

	bool m_explorerBool0;
	bool m_explorerBool1;
	bool m_explorerBool2;

	char padding[0x71];
};
