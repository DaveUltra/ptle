#pragma once

#include "EIGameInstance.h"


class EICamera : public EIGameInstance
{
public:

	Vector3f unknown0;
	Vector3f unknown1;
	Vector3f m_boundsCenter;

	char padding3[0x8];

	Matrix4f m_cameraMatrix;

	float m_boundsSize;
	bool unknown3;

	uint32_t unknown4;
	bool unknown5;
	bool unknown6;
	bool unknown7;
	bool unknown8;
};
