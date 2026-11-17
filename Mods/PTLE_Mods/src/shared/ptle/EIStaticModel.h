#pragma once

#include "EIGameInstance.h"

#include "math/Matrix4f.h"

class ERModel;


class EIStaticModel : public EIGameInstance
{
public:

	static const uint32_t VTABLE = 0x8A58A0;


	Matrix4f m_worldTransform;
	Matrix4f m_inverseWorldTransform;

	bool bool0;
	bool bool1;
	bool bool2;
	bool bool3;

	uint32_t m_modelCRC;

	char padding3[0x14];

	ERModel* m_modelResource;

	char padding4[0x80];


	virtual void UpdateBounds();
	virtual Matrix4f* func_unknown51();
};
