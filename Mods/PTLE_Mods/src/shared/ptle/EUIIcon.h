#pragma once

#include "EUIObjectNode.h"

class ERShader;


class EUIIcon : public EUIObjectNode
{
public:

	char padding6[0x08];

	int unknown8;
	int m_colorIndex0;
	int m_colorIndex1;

	char padding7[0x14];

	ERShader* m_shaders[2];
	float unknown9;
};
