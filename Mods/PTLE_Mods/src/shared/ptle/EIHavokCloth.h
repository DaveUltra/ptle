#pragma once

#include "EIGameInstance.h"

class ERShader;


class EIHavokCloth : public EIGameInstance
{
public:

	static const uint32_t VTABLE = 0x8A68D0;


	char* m_name;
	uint32_t m_shaderCRC;

	int m_rowLength;
	int m_columnLength;
	float m_clothWidth;
	float m_clothHeight;

	void* m_hkCloth;
	int m_numClothPoints;

	Vector4f* unknown0;
	int unknown1;

	ERShader* m_shader;
	float* unknown2;
	Vector3f* unknown3;

	char padding[0xEC];
};
