#pragma once


struct command_t
{
	const char* name;
	void (*func)();
};


extern const command_t commands[];

extern const int NUM_COMMANDS;

void command_execute( int id );

void command_create_window();
void command_process_window();
