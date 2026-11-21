#pragma once

#include "EUIObjectNode.h"

#include "EUIModel.h"
#include "EUIHudCube.h"
#include "EAnimController.h"


class EGamesUI3dScore : public EUIObjectNode
{
public:

	float m_timer1;
	float m_timer2;

	bool unknown0;
	bool unknown1;
	bool unknown2;
	bool unknown3;
	bool unknown4;

	int m_state;

	bool unknown5;

	uint32_t m_score2;
	uint32_t m_score;
	uint32_t m_startScore;
	uint32_t m_endScore;

	EUIModel* m_uiModel;
	EAnimController m_animController;

	char padding0[0x14];

	Matrix4f unknown6;
	Vector3f m_cubesPosition;
	Vector3f unknown7;
	EUIHudCube* m_cubes[2];
	float m_cubeData[2][3];

	char padding1[0x08];
};
