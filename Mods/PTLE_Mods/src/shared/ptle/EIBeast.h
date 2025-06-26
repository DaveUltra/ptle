#pragma once

#include "EICharacter.h"
#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"

class EResource;
class EBeastState;
class EBeastTransition;
class EBeastStateMachine;


class EIBeast : public EICharacter
{
public:

	uint32_t m_beastTypeCRC;
	uint32_t m_modelCRC;

	char padding3[0x4];

	Vector3f m_position;
	float m_orientationAngle;
	char* m_beastTypeName;
	Vector4f m_rotation;
	float m_uniformScale;

	char padding5[0x58];

	EICharacter* m_targetInstance;
	Vector3f m_targetInstancePosition;

	char padding6[0x1C];

	float m_angle;
	float unknown2;
	float unknown3;

	int m_stateIndexCurrent;
	int unknown4;
	int m_stateIndexNext;
	int unknown5;
	EBeastState* m_statePtr;
	int m_numTransitions;
	EBeastTransition* m_arrayTransitions;

	char padding7[0xC0];

	uint32_t m_animMoveCRC;
	uint32_t m_animIdleCRC;
	uint32_t m_animTurnLCRC;
	uint32_t m_animTurnAroundCRC;
	uint32_t m_animTurnRCRC;

	char padding8[0xA8];

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
	virtual void func_unknown69();
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
};
