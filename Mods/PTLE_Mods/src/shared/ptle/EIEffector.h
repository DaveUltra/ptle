#pragma once

#include "EIStaticModel.h"

class ERScript;


class EIEffector : public EIStaticModel
{
public:

	char padding5[0x20];

	uint32_t m_scriptCRC0;
	uint32_t m_scriptCRC1;
	ERScript* m_script0;
	ERScript* m_script1;

	char padding6[0x30];
};
