#pragma once


class ShamanShop
{
public:

	enum PriceSlot
	{
		HEALTH1,
		HEALTH2,
		HEALTH3,
		HEALTH4,
		HEALTH5,
		CANTEEN1,
		CANTEEN2,
		CANTEEN3,
		CANTEEN4,
		CANTEEN5,
		SMASH_STRIKE,
		SUPER_SLING,
		BREAKDANCE,
		JUNGLE_NOTES,
		NATIVE_NOTES,
		CAVERN_NOTES,
		MOUTAIN_NOTES,
		MYSTERY_ITEM,

		NUM_ITEMS,   // Should be equal to 18.

		UNKNOWN = -1,
	};

public:

	static int getPrice( PriceSlot slot );

	static bool setPrice( PriceSlot slot, int numIdols );
};
