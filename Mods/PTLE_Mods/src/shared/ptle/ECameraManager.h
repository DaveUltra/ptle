#pragma once

class EICamera;


class ECameraManager
{
public:

	char padding0[0x5d54];

	float m_fov;
	float unknown0;
	float m_nearPlane;
	float m_farPlane;

	char padding1[0x38];

	float m_shakeDuration;
	float m_shakeIntensity;

	char padding2[0x58];

	float m_fogR;
	float m_fogG;
	float m_fogB;
	bool m_fogEnabled;

	char padding3[0x5F7];

	EICamera* m_defaultFixedCamera;
	EICamera* m_lookCamera;
	EICamera* m_defaultFollowCamera;
	EICamera* m_defaultUfoCamera;
	EICamera* m_slingAimCamera;
	EICamera* m_defaultCinematicCamera;
	EICamera* m_swimufoCamera;
	EICamera* m_defaultPathCamera;
	EICamera* m_followTargetOnlyCamera;
	EICamera* m_watchCamera;
	EICamera* m_tetheredCamera;
	EICamera* m_defaultPathFollowCamera;

	char padding4[0x4];

	EICamera* m_defaultPogoCamera;
	EICamera* m_unknown;


	virtual ~ECameraManager();
};
