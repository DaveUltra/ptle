#pragma once

#include "EItemBase.h"

class EIParticleEmit;
class EIPointLight;


class EItemTorch : public EItemBase
{
public:

	char padding0[0x10];

	EIParticleEmit* m_emitter0;
	EIParticleEmit* m_emitter1;
	EIParticleEmit* m_emitter2;
	EIPointLight* m_pointLight;
};
