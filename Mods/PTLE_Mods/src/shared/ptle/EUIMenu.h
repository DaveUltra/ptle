#pragma once

#include "EUIObjectNode.h"

class ERShader;


class EUIMenu : public EUIObjectNode
{
public:

	EUIObjectNode* m_selectedNode;
	ERShader* m_shader;

	char padding[0xA4];
};
