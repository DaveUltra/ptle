#pragma once

#include "EIBeast.h"


class EISupai : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x8999C8;


	char padding0[0x30];

	uint32_t m_boneJaw;
	Matrix4f unknown0;
	Matrix4f unknown1;

	uint32_t m_boneClaw_R;
	Matrix4f unknown2;
	Matrix4f unknown3;

	uint32_t m_boneClaw_L;
	Matrix4f unknown4;
	Matrix4f unknown5;

	char padding1[0xC0];

	bool unknown6;
	bool m_isTutorialSupai;
	bool unknown7;

	uint32_t m_boneEye_L;
	uint32_t m_boneEye_R;

	char padding2[0x4];

	uint32_t m_boneElbow_R;
	uint32_t m_boneElbow_L;
	uint32_t m_boneKnee2_R;
	uint32_t m_boneKnee2_L;
	uint32_t m_boneAnkle_R;
	uint32_t m_boneAnkle_L;

	EIParticleEmit* unknown8;
	EIParticleEmit* unknown9;
	EIParticleEmit* unknown10;
	EIParticleEmit* unknown11;

	char padding8[0x38];
};
