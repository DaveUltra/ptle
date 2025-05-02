#pragma once

#include "ptle/math/Matrix4f.h"

#include <stdint.h>


struct cheat_t
{
	const char* name;
	void (*func)(bool);
};

struct command_t
{
	const char* name;
	void (*func)();
};


struct savepoint_t
{
	uint32_t levelID;
	Matrix4f playerTransform;
};


extern const cheat_t cheats[];
extern const command_t commands[];
extern savepoint_t save_slots[];

extern const int NUM_CHEATS;
extern const int NUM_COMMANDS;


void command_execute( int id );

void command_create_window();
void command_process_window();
