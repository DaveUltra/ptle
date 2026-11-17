#pragma once

#include "EUIIcon.h"

class ERFont;


class EUITextIcon : public EUIIcon
{
public:

	static const uint32_t VTABLE = 0x8A64B0;


	ERFont* m_font;
	
	char padding7[0x14];

	int m_colorIndex;
	short unknown4;

	float unknown5;

	/// These two actually make up a small struct.
	void* unknown6;
	wchar_t** m_text;
};
