#pragma once

#include "EScriptData.h"

class EResource;


class ESDResource : public EScriptData
{
public:

	static const uint32_t VTABLE = 0x8A8CA8;


	EResource* m_resource;
	char* m_name;
};
