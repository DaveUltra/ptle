#pragma once

#include "EIAnimLod.h"


class EIPlant : public EIAnimLod
{
public:

	bool unknown0;
	bool unknown1;
	bool unknown2;
	bool unknown3;

	int m_state0;
	int m_state1;

	float unknown4;
	float unknown5;
	float unknown6;

	EResource* m_plantQuickdatas;
	void* m_animData;
	Vector3f m_scale;
	float unknown7;

	int m_stateMachine[3][4];
};
