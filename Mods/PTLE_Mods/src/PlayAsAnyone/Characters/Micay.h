#pragma once

#include "CharacterInfo.h"


class CharacterMicay : public CharacterInfo
{
public:

	CharacterMicay();

	virtual void postProcessPose( Transform* destPose, const Transform* harryPose ) const override;
};
