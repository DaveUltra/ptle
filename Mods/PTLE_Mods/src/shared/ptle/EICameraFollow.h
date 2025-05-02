#pragma once

#include "EICameraPit.h"


class EICameraFollow : public EICameraPit
{
public:

	float m_targetYaw;
	float m_targetDistance;
	float m_targetPitch;
	float m_distance;
	float m_pitch;
	float m_yaw;

	char padding6[0x678];
};
