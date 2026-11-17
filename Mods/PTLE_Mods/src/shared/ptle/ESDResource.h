#pragma once

#include "EScriptData.h"

class EResource;


class ESDResource : public EScriptData
{
public:

	EResource* m_resource;
	char* m_name;
};
