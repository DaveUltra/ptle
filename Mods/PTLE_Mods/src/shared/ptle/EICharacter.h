#pragma once

#include "EIGameInstance.h"

#include "math/Matrix4f.h"
#include "math/Vector4f.h"
#include "math/Vector3f.h"

class ERCharacter;


class EICharacter : public EIGameInstance
{
public:

	/*-----*/ char padding3[0x10];

	// This is a matrix array.
	Matrix4f* m_pose;

	int unused3;
	ERCharacter* m_characterResource;

	/*-----*/ char padding4[0x48];

	bool m_onGround;
	Vector4f unused4;
	int unused5;
	int unused6;

	Vector3f m_position;
	Vector3f m_rotationEuler;
	Vector3f m_scale;

	int unused7;
	int unused8;

	Matrix4f m_transformMatrix;

	/*-----*/ char padding5[0x90];

	uint32_t m_collisionFilter;

	/*-----*/ char padding6[0xBC];
};
