#pragma once

#include "EIBeast.h"


class EIBushNinja : public EIBeast
{
public:

	static const uint32_t VTABLE = 0x86D1D8;


	Matrix4f unknown0;

	uint32_t m_bushHashes[3];
	uint32_t m_batModelCRC;
	uint32_t m_batParticlesCRC;
	EIBeast* m_bushes[3];

	char padding0[0x10];

	uint32_t m_boneHandHold_R;
	class EINinjaBat* m_ninjaBat;

	char padding1[0xE8];
};
