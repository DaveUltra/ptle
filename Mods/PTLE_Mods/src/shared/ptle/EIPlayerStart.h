#pragma once

#include "EIStaticModel.h"

#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"
#include "ptle/math/Matrix4f.h"


class EIPlayerStart : public EIStaticModel
{
public:

	static const uint32_t VTABLE = 0x8732C8;


	Vector3f unknown0;
	int unknown1;
	Matrix4f unknown2;
	int unknown7;
	int unknown8;
	int unknown9;
	int unknown10;
};
