#pragma once

#include "ptle/containers/TreeMap/TreeMap.h"
#include <stdint.h>

class EInstance;
class ESaveGameVar;


class ESaveGameMan
{
public:

	TreeMap m_saveLocation1;
	TreeMap m_saveLocation2;


	virtual ~ESaveGameMan();

	virtual void func_unknown0( void* buffer, int param, bool param2 );
	virtual void func_unknown1( void* buffer, int bufferSize, bool param2, bool param3 );
	virtual void func_unknown2( bool param );
	virtual void func_unknown3( bool param );
	virtual void func_unknown4( bool param );


public:  // Custom helper functions.

		/// Get a game variable, either using a name or the name's hash.
	bool GetVar( EInstance* inst, const char* varName,     int*   outValue );
	bool GetVar( EInstance* inst, const char* varName,     float* outValue );
	bool GetVar( EInstance* inst, uint32_t    varNameHash, int*   outValue );
	bool GetVar( EInstance* inst, uint32_t    varNameHash, float* outValue );

		/// Set a game variable, either using a name or the name's hash.
	void SetVar( EInstance* inst, const char* varName,     int   value );
	void SetVar( EInstance* inst, const char* varName,     float value );
	void SetVar( EInstance* inst, uint32_t    varNameHash, int   value );
	void SetVar( EInstance* inst, uint32_t    varNameHash, float value );

		/// Get a variable tied to an instance, from any level.
	bool GetVar( uint32_t levelCRC, uint32_t instanceID, const char* varName,     int*   outValue, bool imm = false );
	bool GetVar( uint32_t levelCRC, uint32_t instanceID, const char* varName,     float* outValue, bool imm = false );
	bool GetVar( uint32_t levelCRC, uint32_t instanceID, uint32_t    varNameHash, int*   outValue, bool imm = false );
	bool GetVar( uint32_t levelCRC, uint32_t instanceID, uint32_t    varNameHash, float* outValue, bool imm = false );

		/// Set a variable tied to an instance, from any level.
	void SetVar( uint32_t levelCRC, uint32_t instanceID, const char* varName,     int   value,     bool imm = false );
	void SetVar( uint32_t levelCRC, uint32_t instanceID, const char* varName,     float value,     bool imm = false );
	void SetVar( uint32_t levelCRC, uint32_t instanceID, uint32_t    varNameHash, int   value,     bool imm = false );
	void SetVar( uint32_t levelCRC, uint32_t instanceID, uint32_t    varNameHash, float value,     bool imm = false );
};
