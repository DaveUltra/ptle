#pragma once

#include "EIDynamicModel.h"


class EITreasureIdol : public EIDynamicModel
{
public:

	int unknown0;
	int unknown1;

	uint32_t m_itemHash;
	bool m_frozen;

	EInstance* m_particleEmitter;

	int unknown2;
	int unknown3;
	bool m_collected;

	char padding6[0x13];


	virtual bool IsStillThere();
	virtual int GetAmount();
	virtual void SetCollected();
};
