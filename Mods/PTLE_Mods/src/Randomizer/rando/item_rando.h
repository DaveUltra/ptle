#pragma once

#include <ostream>
#include <stdint.h>


extern uint32_t g_itemRando_harryActions;


void item_rando_init();

void log_item_rando( std::ostream& os );

void get_item_locations( uint32_t (&out)[8] );
