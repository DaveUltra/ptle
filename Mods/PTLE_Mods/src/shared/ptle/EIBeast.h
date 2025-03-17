#pragma once

#include "EICharacter.h"
#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"

class EResource;


class EIBeast : public EICharacter
{
public:

	uint32_t m_beastTypeCRC;

	char padding3[0x8];

	Vector3f m_position;
	float m_orientationAngle;
	char* m_beastTypeName;
	Vector4f m_rotation;

	char padding5[0x22C];

	EResource* m_beastType;

	char padding6[0x1A0];
};
