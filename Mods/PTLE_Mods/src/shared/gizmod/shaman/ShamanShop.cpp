#include "ShamanShop.h"

#include "utils/func.h"


static const unsigned int PRICES_OFFSET[] = { 1, 2, 3, 4, 5, 23, 24, 25, 26, 27, 9, 11, 7, 15, 17, 13, 19, 21 };

enum PriceOffsets
{
	OFF_HEALTH1 = 1,
	OFF_HEALTH2 = 2,
	OFF_HEALTH3 = 3,
	OFF_HEALTH4 = 4,
	OFF_HEALTH5 = 5,

	OFF_CANTEEN1 = 23,
	OFF_CANTEEN2 = 24,
	OFF_CANTEEN3 = 25,
	OFF_CANTEEN4 = 26,
	OFF_CANTEEN5 = 27,

	OFF_HEALTH = OFF_HEALTH1,
	OFF_CANTEEN = OFF_CANTEEN1,

	OFF_SMASH_STRIKE = 9,
	OFF_SUPER_SLING = 11,
	OFF_BREAKDANCE = 7,

	OFF_JUNGLE_NOTES = 15,
	OFF_NATIVE_NOTES = 17,
	OFF_CAVERN_NOTES = 13,
	OFF_MOUNTAIN_NOTES = 19,

	OFF_MYSTERY_ITEM = 21,
};


/* Valid chunks :
"health"
"water"
"heroic_smash"
"heroic_super_sling"
"heroic_breakdance"
"map_junglea"
"map_jungleb"
"map_caverns"
"map_snow"
"pitfall_cart"
*/
GET_METHOD( 0x63A720, void*, ERQuickdata_FindChunkByName, void*, char* );
GET_METHOD( 0x63A8D0, void*, ERQuickdata_FindInChunk, void*, void*, char* );
static int* get_prices_buffer()
{
	// Shaman prices are contained in a quickdatas resource, which is loaded at the start of the game.
	// This resource can be accessed through 3 globals. We're using the one that comes first (0x910638).
	// Then, in the prices chunk, we get the health prices. Prices area actually contiguous in memory,
	// and health is the one that comes first. So we retrieve its address, and access other prices relative
	// to it. Refer to PriceOffsets enum for more details.

	void* uiQuickdatas = *((void**) 0x910638);
	if ( !uiQuickdatas ) {
		return 0;
	}

	void* storeChunk = ERQuickdata_FindChunkByName( uiQuickdatas, "UIDDStoreItemDef" );
	int** storeResourcePtr = (int**) ERQuickdata_FindInChunk( uiQuickdatas, storeChunk, "health" );

	int* prices = storeResourcePtr[10] - 1;
	return prices;
}


int ShamanShop::getPrice( PriceSlot slot )
{
	int* prices = get_prices_buffer();
	if ( !prices ) return -1;

	return prices[PRICES_OFFSET[slot]];
}

bool ShamanShop::setPrice( PriceSlot slot, int numIdols )
{
	int* prices = get_prices_buffer();
	if ( !prices ) return false;

	prices[PRICES_OFFSET[slot]] = numIdols;
	return true;
}
