#pragma once

#include "CharacterInfo.h"


class CharacterBittenbinder : public CharacterInfo
{
public:

	CharacterBittenbinder();

	virtual void postProcessPose( Transform* destPose, const Transform* harryPose ) const;
};
