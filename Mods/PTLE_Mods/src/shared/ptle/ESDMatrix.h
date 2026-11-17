#pragma once

#include "EScriptData.h"

#include "ptle/math/Matrix4f.h"


class ESDMatrix : public EScriptData
{
public:

	Matrix4f m_value;
};
