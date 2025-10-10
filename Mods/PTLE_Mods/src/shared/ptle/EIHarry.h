#pragma once

#include "EIPlayer.h"

class EIStaticModel;
class EIParticleEmitter;


class EIHarry : public EIPlayer
{
public:

	char padding6[0x688];

	Vector3f m_bodyRotation;
	Vector3f m_bodyTargetRotation;

	char padding10[0x730];

	int m_idolsCollected;

	int      m_risingStrike;
	uint32_t m_risingStrikeHash;
	int      m_superSling;
	uint32_t m_superSlingHash;
	int      m_breakdance;
	uint32_t m_breakdanceHash;
	int      m_heroicDash;
	uint32_t m_heroicDashHash;
	int      m_heroicDive;
	uint32_t m_heroicDiveHash;
	int      m_smashStrike;
	uint32_t m_smashStrikeHash;

	char padding7[0x24];

	EIParticleEmitter* m_particleEmitters[10];

	char padding8[0x1C];

	EICharacter* m_playerIceCube;
	EIStaticModel* m_modelShockEffect;

	char padding9[0x10];

	int m_powerGloves;

	char padding11[0x68];
};
