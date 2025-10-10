#include "gizmod/Gizmod.h"

#include "gizmod/shaman/ShamanShop.h"

#include "rando.h"
#include "utils/log.h"
#include "utils/func.h"

#include <vector>
#include <stdint.h>


#define NUM_PRICES 18

static const int      MAX_IDOLS = 138;
static const int      DEFAULT_SHOP_PRICES[NUM_PRICES] = { 2, 4, 8, 16, 32, 1, 2, 3, 4, 5, 10, 10, 10, 9, 7, 7, 8, 0 };
static const int      MAPLESS_SHOP_PRICES[]           = {    4, 8, 16, 32,       3, 4, 5, 10, 10, 10, 9, 7, 7, 8    };


// Our generated prices.
int shaman_shop_prices[NUM_PRICES];


static int sort_compare( const void* l, const void* r )
{
	int i0 = *reinterpret_cast<const int*>(l);
	int i1 = *reinterpret_cast<const int*>(r);
	return i0 - i1;
}

static void shuffle_prices()
{
	const int* prices = rando_config.disableMapsInShop ? MAPLESS_SHOP_PRICES : DEFAULT_SHOP_PRICES;
	int max = rando_config.disableMapsInShop ? (NUM_PRICES - 4) : (NUM_PRICES);

	if ( rando_config.randomizeShamanShop || rando_config.disableMapsInShop ) {
		memcpy( shaman_shop_prices, prices, max * sizeof(int) );
	}

	for ( int n = max - 1; n >= 1; n-- ) {
		int i = rand() % (n + 1);

		int tmp = shaman_shop_prices[i];
		shaman_shop_prices[i] = shaman_shop_prices[n];
		shaman_shop_prices[n] = tmp;
	}

	// Sort health and canteen prices.
	std::qsort( shaman_shop_prices,     5, sizeof(int), sort_compare );
	std::qsort( shaman_shop_prices + 5, 5, sizeof(int), sort_compare );
}


// Generates random prices, but does not actually apply it to the game's data.
void init_shaman_shop()
{
	shuffle_prices();

	Gizmod::getInstance()->getLogger()->log_printf( "Initialized shaman prices.\n" );
}

// Applies the custom prices to the actual in-memory shaman shop.
// This is only required once.
void patch_shaman_shop()
{
	if ( rando_config.disableMapsInShop ) {
		InventoryItem::setNotesUnlocked( InventoryItem::JUNGLE_NOTES, true );
		InventoryItem::setNotesUnlocked( InventoryItem::NATIVE_NOTES, true );
		InventoryItem::setNotesUnlocked( InventoryItem::CAVERN_NOTES, true );
		InventoryItem::setNotesUnlocked( InventoryItem::MOUNTAIN_NOTES, true );

		for ( int i = 0; i < ShamanShop::JUNGLE_NOTES; i++ ) {
			ShamanShop::setPrice( (ShamanShop::PriceSlot) i, shaman_shop_prices[i] );
		}
		ShamanShop::setPrice( ShamanShop::MYSTERY_ITEM, shaman_shop_prices[NUM_PRICES - 5] );
	}
	else {
		for ( int i = 0; i < NUM_PRICES; i++ ) {
			ShamanShop::setPrice( (ShamanShop::PriceSlot) i, shaman_shop_prices[i] );
		}
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
	if ( !rando_config.disableMapsInShop ) {
		os << " - Jungle Notes    : " << shaman_shop_prices[13] << '\n';
		os << " - Native Notes    : " << shaman_shop_prices[14] << '\n';
		os << " - Caverns Notes   : " << shaman_shop_prices[15] << '\n';
		os << " - Mountains Notes : " << shaman_shop_prices[16] << '\n';
		os << " - Mystery Item    : " << shaman_shop_prices[17] << '\n';
	}
	else {
		os << " - Mystery Item    : " << shaman_shop_prices[13] << '\n';
	}
	os << '\n';
}
