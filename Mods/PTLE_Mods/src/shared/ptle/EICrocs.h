#pragma once

#include "EIBeast.h"


class EICrocs : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x86FD18;


	char padding0[0x20];

	Matrix4f m_matJawTop;
	int m_boneJawTop;
	Matrix4f m_matJawBottom;
	int m_boneJawBottm;
	Matrix4f m_matBip01;
	int m_boneBip01;

	char padding1[0xEC];
};
