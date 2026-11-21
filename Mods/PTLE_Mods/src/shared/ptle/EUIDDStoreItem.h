#pragma once

#include "EUITextIcon.h"

class EResource;
class ERShader;


class EUIDDStoreItem : public EUITextIcon
{
public:

	void* m_storeResourcePtr;

	bool unknown0;

	float unknown1;
	float unknown2;

	EResource* m_uiQuickdata;

	int unknown3;
	float unknown4;

	ERShader* m_shaderIcon;

	bool unknown5;

	float unknown6[2];
	float unknown7[2];
	float unknown8;
	float unknown9;
	float unknown10;
	float unknown11;

	int m_itemPrice;

	char padding[0x44];
};
