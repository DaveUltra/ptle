#pragma once

#include "EIGameInstance.h"

class ERShader;


class EIShadowSpot : public EIGameInstance
{
public:

	static const uint32_t VTABLE = 0x896990;


	Vector3f m_aabbCenter;

	int unknown0;
	int unknown1;

	float unknown2;
	float m_aabbSize;
	float unknown3;

	ERShader* m_shader;

	char padding[0x5C];
};
