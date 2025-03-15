#include <vector>

#define NUM_PRICES 18

static const int MAX_IDOLS = 138;
static const int DEFAULT_SHOP_PRICES[NUM_PRICES] = { 2, 4, 8, 16, 32, 1, 2, 3, 4, 5, 10, 10, 10, 9, 7, 7, 8, 0 };


int shaman_shop_prices[NUM_PRICES];

int prices_health_buffer[] = {
	5, 2, 4, 8, 16, 32
};
int prices_canteen_buffer[] = {
	5, 1, 2, 3, 4, 5
};
int prices_skills_buffer[] = {
	1, 10,
	1, 10,
	1, 10
};
int prices_maps_buffer[] = {
	1, 9,
	1, 7,
	1, 7,
	1, 8
};
int prices_mystery_buffer[] = {
	1, 0
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

	for ( int n = NUM_PRICES; n >= 1; n-- ) {
		int i = rand() % (n + 1);

		int tmp = shaman_shop_prices[i];
		shaman_shop_prices[i] = shaman_shop_prices[n];
		shaman_shop_prices[n] = tmp;
	}
}

void randomize_shaman_shop()
{
	shuffle_prices();

	// Sort health and canteen prices.
	std::qsort( shaman_shop_prices,     5, sizeof(int), sort_compare );
	std::qsort( shaman_shop_prices + 5, 5, sizeof(int), sort_compare );
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

#include "injector/injector.hpp"
void iiii()
{
	// Patch instruction that sets the pointer to the prices table.
	injector::MakeNOP( 0x5BD642, 6 );
	//injector::MakeJMP( 0x5BD642, patch_shaman_shop );
}
