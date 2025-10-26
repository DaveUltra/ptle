#pragma once

#include "EICameraPit.h"

#include "ptle/math/Vector3f.h"


class EICameraFollow : public EICameraPit
{
public:

	float m_targetYaw;
	float m_targetDistance;
	float m_targetPitch;
	float m_distance;
	float m_pitch;
	float m_yaw;

	Vector3f unknown13;
	Vector3f unknown14;
	float unknown15;
	Vector3f unknown16;
	Vector3f unknown17;
	Vector3f unknown18;
	Vector3f unknown19;

	float unknown20[31][8];   // Likely an array of 31 structs of size 32 bytes, containing mostly floats.

	uint32_t unknown21[5];

	int unknown22;
	bool unknown23;

	char unknown24[4][40];    // Likely an array of 4 structs of size 40 bytes.

	char padding6[0x190];
};
