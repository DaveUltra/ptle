#pragma once

#include "containers/LinkedList/LinkedList.h"
#include "containers/HashMap/HashMap.h"

#include "math/Vector3f.h"


struct SoundChannel
{
	char padding[0x38];
};

struct SoundStructUnknown
{
	char padding[0x9D0];
};

class ESfxManager
{
public:

	virtual ~ESfxManager();

	SoundChannel m_channels[32];
	SoundStructUnknown unknown0;

	char padding0[0x480];

	LinkedList m_activeVoices;

	Vector3f unknown1[5];
	Vector3f unknown2[5];

	char padding1[0x70];

	float m_invSpeedOfSound;
	bool unknown3;
	int unknown4;
	HashMap m_channelBySoundMap;

	char padding2[0x2C];
};
