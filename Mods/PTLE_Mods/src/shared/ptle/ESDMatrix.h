#pragma once

#include "EScriptData.h"

#include "ptle/math/Matrix4f.h"


class ESDMatrix : public EScriptData
{
public:

	static const uint32_t VTABLE = 0x8AE860;


	Matrix4f m_value;
};
