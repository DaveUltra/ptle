#pragma once

#include "EScriptData.h"

#include "ptle/math/Vector3f.h"


class ESDVector : public EScriptData
{
public:

	static const uint32_t VTABLE = 0x8A8D5C;


	Vector3f m_value;
};
