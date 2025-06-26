#pragma once

#include "EIStaticModel.h"

#include "ptle/math/Vector3f.h"
#include "ptle/math/Vector4f.h"

class ERScript;


class EIEffector : public EIStaticModel
{
public:

	bool unknown2;

	Vector3f m_position;
	Vector4f m_rotation;

	uint32_t m_scriptCRC0;
	uint32_t m_scriptCRC1;
	ERScript* m_script0;
	ERScript* m_script1;

	bool unknown3;

	float unknown4;

	bool m_active;
	bool m_frontActive;
	bool m_backActive;

	char padding6[0x09];

	EInstance* unknown5;
	Vector3f m_scale;

	char padding7[0x0C];
};
