#include "rando.h"
#include "utils/log.h"
#include "utils/func.h"

#include <vector>
#include <stdint.h>


#define NUM_PRICES 18

static const int      MAX_IDOLS = 138;
static const int      DEFAULT_SHOP_PRICES[NUM_PRICES] = { 2, 4, 8, 16, 32, 1, 2, 3, 4, 5, 10, 10, 10, 9, 7, 7, 8, 0 };
static const uint32_t PRICES_OFFSET[NUM_PRICES] = { 1, 2, 3, 4, 5, 23, 24, 25, 26, 27, 9, 11, 7, 15, 17, 13, 19, 21 };


// Our generated prices.
int shaman_shop_prices[NUM_PRICES];


enum shaman_price_offset_t
{
	PRICE_HEALTH1 = 1,
	PRICE_HEALTH2 = 2,
	PRICE_HEALTH3 = 3,
	PRICE_HEALTH4 = 4,
	PRICE_HEALTH5 = 5,

	PRICE_CANTEEN1 = 23,
	PRICE_CANTEEN2 = 24,
	PRICE_CANTEEN3 = 25,
	PRICE_CANTEEN4 = 26,
	PRICE_CANTEEN5 = 27,

	PRICE_HEALTH = PRICE_HEALTH1,
	PRICE_CANTEEN = PRICE_CANTEEN1,

	PRICE_SMASH_STRIKE = 9,    //
	PRICE_SUPER_SLING = 11,    // Heroic abilities.
	PRICE_BREAKDANCE = 7,      //

	PRICE_MAP_JUNGLE = 15,     //
	PRICE_MAP_NATIVE = 17,     // Area
	PRICE_MAP_CAVERNS = 13,    // notes.
	PRICE_MAP_MOUNTAINS = 19,  //

	PRICE_MYSTERY = 21,        // Mystery item (Pitfall! game in Punchau Shrine).
};


static int sort_compare( const void* l, const void* r )
{
	int i0 = *reinterpret_cast<const int*>(l);
	int i1 = *reinterpret_cast<const int*>(r);
	return i0 - i1;
}

static void shuffle_prices()
{
	memcpy( shaman_shop_prices, DEFAULT_SHOP_PRICES, NUM_PRICES * sizeof(int) );

	for ( int n = NUM_PRICES - 1; n >= 1; n-- ) {
		int i = rand() % (n + 1);

		int tmp = shaman_shop_prices[i];
		shaman_shop_prices[i] = shaman_shop_prices[n];
		shaman_shop_prices[n] = tmp;
	}
}

/*"health";
"water";
"heroic_smash";
"heroic_super_sling";
"heroic_breakdance";
"map_junglea";
"map_jungleb";
"map_caverns";
"map_snow";
"pitfall_cart";*/

GET_METHOD( 0x63A720, void*, FindChunkByName, void*, char* );
GET_METHOD( 0x63A8D0, void*, FindInChunk, void*, void*, char* );
static int* get_prices_buffer()
{
	// Shaman prices are contained in a quickdatas resource, which is loaded at the start of the game.
	// This resource can be accessed through 3 globals. We're using the one that comes first (0x910638).
	// Then, in the prices chunk, we get the health prices. Prices area actually contiguous in memory,
	// and health is the one that comes first. So we retrieve its address, and access other prices relative
	// to it. Refer to shaman_price_offset_t for more details.

	void* uiQuickdatas = *((void**) 0x910638);
	void* storeChunk = FindChunkByName( uiQuickdatas, "UIDDStoreItemDef" );
	int** storeResourcePtr = (int**) FindInChunk( uiQuickdatas, storeChunk, "health" );

	int* prices = storeResourcePtr[10] - 1;
	return prices;
}


// Generates random prices, but does not actually apply it to the game's data.
void randomize_shaman_shop()
{
	shuffle_prices();

	// Sort health and canteen prices.
	std::qsort( shaman_shop_prices,     5, sizeof(int), sort_compare );
	std::qsort( shaman_shop_prices + 5, 5, sizeof(int), sort_compare );

	log_printf( "Initialized shaman prices.\n" );
}

// Applies the custom prices to the actual in-memory shaman shop.
// This is only required once.
void patch_shaman_shop()
{
	int* pricesBuffer = get_prices_buffer();

	for ( int i = 0; i < NUM_PRICES; i++ ) {
		pricesBuffer[PRICES_OFFSET[i]] = shaman_shop_prices[i];
	}
}

void log_shaman_shop( std::ostream& os )
{
	if ( !rando_config.randomizeShamanShop ) return;

	os << "Shaman prices :\n";
	os << " - Health          : " << shaman_shop_prices[0] << ", " << shaman_shop_prices[1] << ", " << shaman_shop_prices[2] << ", " << shaman_shop_prices[3] << ", " << shaman_shop_prices[4] << '\n';
	os << " - Canteen         : " << shaman_shop_prices[5] << ", " << shaman_shop_prices[6] << ", " << shaman_shop_prices[7] << ", " << shaman_shop_prices[8] << ", " << shaman_shop_prices[9] << '\n';
	os << " - Smash Strike    : " << shaman_shop_prices[10] << '\n';
	os << " - Super Sling     : " << shaman_shop_prices[11] << '\n';
	os << " - Breakdance      : " << shaman_shop_prices[12] << '\n';
	os << " - Jungle Notes    : " << shaman_shop_prices[13] << '\n';
	os << " - Native Notes    : " << shaman_shop_prices[14] << '\n';
	os << " - Caverns Notes   : " << shaman_shop_prices[15] << '\n';
	os << " - Mountains Notes : " << shaman_shop_prices[16] << '\n';
	os << " - Mystery Item    : " << shaman_shop_prices[17] << '\n';
	os << '\n';
}
