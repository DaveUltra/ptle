#pragma once

#include "EIGameInstance.h"

#include "math/Matrix4f.h"

class ERModel;


class EIStaticModel : public EIGameInstance
{
public:

	char padding3[12];

	Matrix4f m_worldTransform;
	Matrix4f m_inverseWorldTransform;

	bool bool0;
	bool bool1;
	bool bool2;
	bool bool3;

	uint32_t m_modelCRC;

	char padding4[0x14];

	ERModel* m_modelResource;

	char padding5[0x80];
};
