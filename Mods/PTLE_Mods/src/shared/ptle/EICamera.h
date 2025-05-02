#pragma once

#include "EIGameInstance.h"


class EICamera : public EIGameInstance
{
public:

	Vector3f unknown0;
	Vector3f unknown1;
	Vector3f unknown2;

	char padding3[0x8];

	Matrix4f unknown3;

	char padding4[0x8];

	uint32_t unknown4;
	bool unknown5;
	bool unknown6;
	bool unknown7;
	bool unknown8;
};
