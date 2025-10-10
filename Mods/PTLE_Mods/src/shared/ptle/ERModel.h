#pragma once

#include "EResource.h"

#include "ptle/containers/Array/Array.h"
#include "ptle/math/Vector3f.h"
#include "ptle/math/AABB.h"


class ERModel : public EResource
{
public:

	Array m_array0;
	Vector3f unknown0;

	float unknown1;

	AABB m_bounds;

	Vector3f unknown2;
	Vector3f unknown3;

	char padding1[0x08];

	bool unknown4;
	bool unknown5;
	bool unknown6;
	bool unknown7;

	char padding2[0x04];

	Array unknown8[3];

	char padding3[0x04];
};
