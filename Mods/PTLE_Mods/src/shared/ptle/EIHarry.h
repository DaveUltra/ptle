#pragma once

#include "EIPlayer.h"

class EIStaticModel;
class EIParticleEmit;
class EIParticleEmitter;
class EResource;


struct SkinnedModelPart
{
	int unknown0;
	int unknown1;
	float unknown2;
	int unknown3;
	Vector3f* unknown4;
	void* unknown5;
	Matrix4f* unknown6;

	char padding[0xC];

	EICharacter* unknown7;
	EAnimController* unknown8;

	char padding1[0x8];
};

struct HarryStrikeRibbon
{
	Vector3f* unknown0;
	Vector3f unknown1;
	Vector3f unknown2;
	Vector3f* unknown3;
	class EParticleRibbon* m_particleEmitter;
	bool unknown4;
};

struct PowerGloves
{
	bool m_enabled;
	EIParticleEmit* unknown0;
	EIParticleEmit* unknown1;
	EIParticleEmit* unknown2;
	EIParticleEmit* unknown3;
	class EIHarry* m_harry;
	SkinnedModelPart unknown4;
	EResource* unknown5;
	char padding[4];
	float unknown6;
};

class EIHarry : public EIPlayer
{
public:

	static const uint32_t VTABLE = 0x87D300;


	char padding0[0x19C];

	uint32_t m_boneRHandHold;
	uint32_t m_boneLHand;
	uint32_t m_boneRHand;
	uint32_t m_boneSpine1;
	uint32_t m_boneLToe0;
	uint32_t m_boneRToe0;
	uint32_t m_boneSpine2;
	uint32_t m_boneLClavicle;
	uint32_t m_boneRClavicle;
	uint32_t m_boneRUpperArm;
	uint32_t m_boneRForearm;
	uint32_t m_boneLForearm;
	uint32_t m_boneLThigh;
	uint32_t m_boneRThigh;
	uint32_t m_boneNeck1;

	Matrix4f m_matLClavicle;
	Matrix4f m_matRClavicle;
	Matrix4f m_matRHand;
	Matrix4f m_matRToe0;
	Matrix4f m_matLToe0;
	uint32_t* m_animationHashes;

	char padding1[0x364];

	Vector3f m_bodyRotation;
	Vector3f m_bodyTargetRotation;

	char padding12[0x28];

	HarryStrikeRibbon m_strikeRibbons[2];

	char padding10[0x6A8];

	uint32_t m_hitFlags;
	bool m_slingAnimated;
	bool m_dieOnNextLand;

	int unknown0;
	int unknown1;

	int m_idolsCollected;

	// TODO : replace those by EHeroicSkill structures.
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

	PowerGloves m_powerGloves;

	char padding11[0x10];
};
