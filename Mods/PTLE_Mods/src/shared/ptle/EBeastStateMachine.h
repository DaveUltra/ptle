#pragma once

class EIBeast;
class ERScript;


struct MemberFuncCallback
{
	void (*m_func)();
	char zero[0x0C];
};


class EBeastState
{
public:

	void* unknown0;

	char* m_name;

	MemberFuncCallback m_onEnter;
	MemberFuncCallback m_onUpdate;
	MemberFuncCallback m_onLeave;

	char padding3[0x0C];

	int m_index;

	ERScript* m_script0;
	ERScript* m_script1;
	ERScript* m_script2;

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
