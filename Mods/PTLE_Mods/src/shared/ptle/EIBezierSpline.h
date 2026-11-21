#pragma once

#include "EIGameInstance.h"


class EIBezierSpline : public EIGameInstance
{
public:

	static const uint32_t VTABLE = 0x8A6FD0;


	int m_numSegments;

	int unknown0;

	Vector3f* m_arrVec;

	char padding[0x1C];
};
