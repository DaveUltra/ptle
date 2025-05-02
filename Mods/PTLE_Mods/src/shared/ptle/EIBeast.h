#pragma once

#include "EICharacter.h"
#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"

class EResource;


class EIBeast : public EICharacter
{
public:

	uint32_t m_beastTypeCRC;
	uint32_t m_modelCRC;

	char padding3[0x4];

	Vector3f m_position;
	float m_orientationAngle;
	char* m_beastTypeName;
	Vector4f m_rotation;
	float m_uniformScale;

	char padding5[0x228];

	EResource* m_beastType;

	char padding6[0x1A0];


	virtual bool func_unknown60();
	virtual void func_unknown61();
	virtual void func_unknown62();

};
