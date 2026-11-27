#pragma once

#include "EIBeast.h"


class EIEyeballs : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x876800;


	float unknown0;
	int unknown1;
	float m_angleZ;
	Vector3f unknown2;
	uint32_t m_boneEye;

	char padding[0x10];

	Matrix4f m_matrix;

	float unknown3;
	float unknown4;
	bool unknown5;
};
