#pragma once

#include "EIStaticModel.h"

#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"
#include "ptle/math/Matrix4f.h"


class EIPlayerStart : public EIStaticModel
{
public:

	static const uint32_t VTABLE = 0x8732C8;


	Vector3f m_position;
	Matrix4f m_worldMatrix;
	float m_camDistance;
	float m_camPitch;
	float unknown9;
	float unknown10;
};
