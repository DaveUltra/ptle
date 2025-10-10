#pragma once

#include "ptle/ESaveGameMan.h"


	/// Gizmod save manager, not to be mistaken with the game's native save manager.
	///
	/// This class is a layer for the game's save manager, to allow mods to add their
	/// own save data to a game save, without conflicting with existing data, or other
	/// mods.
	/// If you want to modify variables from the game itself, use the native manager.
class SaveManager
{
public:

		/// Get the game's native save game manager.
	static ESaveGameMan* getNativeManager();


		/// Get values for the current mod.
	bool getVar( const char* name, int* outValue );
	bool getVar( const char* name, float* outValue );

		/// Set values for the current mod.
	void setVar( const char* name, int value );
	void setVar( const char* name, float value );
};
