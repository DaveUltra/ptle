#pragma once

#include "EIPlayer.h"


class EIHarry : public EIPlayer
{
public:

	char padding6[0x688];

	Vector3f m_bodyRotation;
	Vector3f m_bodyTargetRotation;

	char padding10[0x730];

	int m_idolsCollected;
	int m_risingStrike;
	int unused4;
	int m_superSling;
	int unused3;
	int m_breakdance;
	int unused0;
	int m_heroicDash;
	int unused1;
	int m_heroicDive;
	int unused2;
	int m_smashStrike;

	char padding7[0xF0];
};
