#pragma once

#include "EInstance.h"

#include "ptle/containers/LinkedList/LinkedList.h"


class EUIObjectNode : public EInstance
{
public:

	LinkedList m_children;
	EUIObjectNode* m_parent;
	EUIObjectNode* unknown0;

	char padding3[0x0C];

	float unknown1;
	float unknown2;
	float unknown3;
	float unknown4;
	float unknown5;
	float unknown6;
	float unknown7;

	char padding4[0x0C];

	char* m_uiTypeName;

	char padding5[0x08];
};
