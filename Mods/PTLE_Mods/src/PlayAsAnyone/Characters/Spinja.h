#pragma once

#include "CharacterInfo.h"


class CharacterSpinja : public CharacterInfo
{
public:

	CharacterSpinja();

	virtual void postProcessPose( Transform* destPose, const Transform* harryPose ) const override;
};
