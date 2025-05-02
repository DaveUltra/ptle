
class ERScript;


class EBeastState
{
public:

	void* unknown0;

	char* m_name;

	void (*m_func0)();
	char padding0[0xC];
	void (*m_func1)();
	char padding1[0xC];
	void (*m_func2)();
	char padding2[0xC];

	char padding3[0x10];

	ERScript* m_script0;
	ERScript* m_script1;
	ERScript* m_script2;

	int unknown1;
};


// True class name is unknown and just assumed based on context.
class EBeastStateMachine
{
public:

	char padding0[0x8];

	EBeastState* m_states;

	char padding1[0x8];

	uint32_t m_numStates;

	char padding2[0x8];

	uint32_t m_refCount;
};
