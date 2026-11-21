#pragma once

#include "EUIObjectNode.h"

#include "ptle/math/Matrix4f.h"
#include "ptle/math/Vector4f.h"

#include <stdint.h>

class EUIModel;


class EUIHudCube : public EUIObjectNode
{
public:

	float unknown0;

	EUIModel* m_uiModel;

	int m_state;

	char padding0[0x8];

	uint32_t m_shaders[4];

	char padding1[0x1C];

	Matrix4f unknown1;
	Vector4f unknown2;
};
