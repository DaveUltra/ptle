#pragma once

#include "EICharacter.h"
#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"

class EResource;
class EBeastState;
class EBeastTransition;
class EBeastStateMachine;

class EIBezierSpline;


class EIBeast : public EICharacter
{
public:

	static const uint32_t VTABLE = 0x86C3D0;

	uint32_t m_beastTypeCRC;
	uint32_t m_modelCRC;

	char padding3[0x4];

	Vector3f m_position;
	float m_orientationAngle;
	char* m_beastTypeName;
	Vector4f m_rotation;
	float m_uniformScale;
	float m_targetAngle;

	Matrix4f m_worldMatrix;
	Vector3f m_previousPosition;

	EICharacter* m_targetInstance;
	Vector3f m_targetInstancePosition;

	char padding6[0x1C];

	float m_angle;
	float unknown2;

	float m_elapsedTime;
	int m_stateIndexCurrent;
	int unknown4;
	int m_stateIndexNext;
	int unknown5;
	EBeastState* m_statePtr;
	int m_numTransitions;
	EBeastTransition* m_arrayTransitions;

	char padding7[0xC4];

	uint32_t m_animMoveCRC;
	uint32_t m_animIdleCRC;
	uint32_t m_animTurnLCRC;
	uint32_t m_animTurnAroundCRC;
	uint32_t m_animTurnRCRC;

	float unknown6;
	float unknown7;
	float unknown8;

	float m_noiseRun;
	float m_noiseWalk;
	float m_noiseSneak;

	float unknown9;

	bool m_enableCollision;

	char padding8[0x13];

	float unknown10;
	EIBezierSpline* m_bezierSpline;
	uint32_t m_bezierSplineHash;

	char padding11[0x48];

	int m_locoBase;
	int m_animIndex;

	char padding12[0x18];

	EResource* m_beastType;  // This is a quickdata, not an ERCharacter.

	char padding9[0x48];

	int m_nextState;
	bool m_forceNextState;

	char padding10[0x153];


	virtual bool func_unknown60();
	virtual void func_unknown61();
	virtual void func_unknown62();
	virtual EBeastStateMachine* GetStateMachine();
	virtual void IncrementStateMachineRef();
	virtual void DecrementStateMachineRef();
	virtual void func_unknown65();
	virtual void func_unknown66();
	virtual void func_unknown67();
	virtual void func_unknown68();
	virtual int GetBossHealth();
	virtual void func_unknown70();
	virtual void EnterState();
	virtual void func_unknown71();
	virtual void ExitState();
	virtual void EnterCutsceneState();
	virtual void UpdateCutsceneState();
	virtual void ExitCutsceneState();

	virtual void SetPositionOnGround();
	virtual void DeserializePart1( void* );
	virtual void DeserializePart2();

	virtual void func_unknown72();
	virtual bool func_unknown73();
	virtual bool func_unknown74();

	virtual bool Cond_HarryLookingAway();
	virtual bool func_unknown75();
	virtual bool func_unknown76();
	virtual bool Cond_IsAnimationDone();
	virtual bool Cond_WaitASec();
	virtual bool Cond_HitByUnknown0();
	virtual bool Cond_HitByUnknown1();
	virtual bool Cond_HitByUnknown2();
	virtual bool func_unknown77();
	virtual bool Cond_IsPlayerInRange();
	virtual bool Cond_IsPlayerNotInRange();
	virtual bool func_unknown78();
	virtual bool func_unknown79();
	virtual bool Cond_IsTargetInRange();
	virtual bool Cond_IsTargetNotInRange();
};
