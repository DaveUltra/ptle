#pragma once

#include "EIGameInstance.h"

#include "ptle/math/Vector4f.h"
#include "ptle/math/Matrix4f.h"


class EIMarker : public EIGameInstance
{
public:

	static const uint32_t VTABLE = 0x8AAE70;


	Vector3f m_position;
	Vector4f m_rotation;

	float unknown0;
	uint32_t m_scriptCRC;

	char padding[0x4];

	ERScript* m_script;
	Matrix4f m_worldMatrix;
};
