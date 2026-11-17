#pragma once

#include "EScriptData.h"


class ESDFloat : public EScriptData
{
public:

	static const uint32_t VTABLE = 0x8A8D00;


	float m_value;
};
