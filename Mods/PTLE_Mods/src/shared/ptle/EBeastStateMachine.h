#pragma once

class EIBeast;
class ERScript;


struct MemberFuncCallback
{
	bool (*m_func)();
	char zero[0x0C];
};


struct BeastStateDataUnknown
{
	uint32_t m_animCRC;
	uint32_t m_soundCRC;
	float unknown0;
	float unknown1;
	uint32_t m_scriptEnterCRC;
	uint32_t m_scriptUpdateCRC;
	uint32_t m_scriptLeaveCRC;
	uint32_t unknown2;
};

class EBeastState
{
public:

	BeastStateDataUnknown* m_stateData;

	char* m_name;

	MemberFuncCallback m_onEnter;
	MemberFuncCallback m_onUpdate;
	MemberFuncCallback m_onLeave;

	int m_animID;   // Supposedly...
	float unknown2;
	float unknown3;

	int m_index;

	ERScript* m_scriptEnter;
	ERScript* m_scriptUpdate;
	ERScript* m_scriptLeave;

	uint32_t unknown1;
};

class EBeastCondition
{
public:

	void* unknown0;

	char* m_name;

	EBeastCondition* unknown2;

	char padding0[0x04];

	MemberFuncCallback m_func;
	int m_index;

	char padding1[0x04];
};

class EBeastTransition
{
public:

	void* m_transitionInfo;
	EBeastCondition* m_condition;
	EBeastState* m_stateA;
	EBeastState* m_stateB;
};


// True class name is unknown and just assumed based on context.
class EBeastStateMachine
{
public:

	void* unknown0;

	char* m_beastTypeName;

	EBeastState*      m_states;
	EBeastCondition*  m_conditions;
	EBeastTransition* m_transitions;

	uint32_t m_numStates;
	uint32_t m_numConditions;
	uint32_t m_numTransitions;

	uint32_t m_refCount;
};
