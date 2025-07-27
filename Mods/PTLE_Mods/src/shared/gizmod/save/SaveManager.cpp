#include "gizmod/Gizmod.h"
#include "SaveManager.h"

#include "gizmod/GizmodPlugin.h"

#include "ptle/ESaveGameMan.h"

#include <string>


static void makeFullName( std::string& fullName, const char* name )
{
	fullName += Gizmod::getThisPlugin()->getSystemName();
	fullName += '.';
	fullName += name;
}


ESaveGameMan* SaveManager::getNativeManager()
{
	return *((ESaveGameMan**) 0x920058);
}

bool SaveManager::getVar( const char* name, int* outValue )
{
	std::string fullName; fullName.reserve(128);
	makeFullName( fullName, name );

	return SaveManager::getNativeManager()->GetVar( (EInstance*) 0, fullName.c_str(), outValue );
}
bool SaveManager::getVar( const char* name, float* outValue )
{
	std::string fullName; fullName.reserve(128);
	makeFullName( fullName, name );

	return SaveManager::getNativeManager()->GetVar( (EInstance*) 0, fullName.c_str(), outValue );
}


void SaveManager::setVar( const char* name, int value )
{
	std::string fullName; fullName.reserve(128);
	makeFullName( fullName, name );

	SaveManager::getNativeManager()->SetVar( 0, fullName.c_str(), value );
}
void SaveManager::setVar( const char* name, float value )
{
	std::string fullName; fullName.reserve(128);
	makeFullName( fullName, name );

	SaveManager::getNativeManager()->SetVar( 0, fullName.c_str(), value );
}
