#include "ptle/ESaveGameMan.h"

#include "ptle/ESaveGameVar.h"
#include "ptle/types/types.h"
#include "utils/func.h"


GET_FUNC( 0x6609E0, int, ESaveGameMan_GetVar, const ETypeInfo*, ESaveGameMan*, EInstance*, uint32_t, uint32_t*, uint32_t* );
GET_FUNC( 0x660B40, void, ESaveGameMan_SetVar, const ETypeInfo*, ESaveGameMan*, EInstance*, uint32_t, uint32_t* );
GET_FUNC( 0x6F8EC0, uint32_t, HashStr_CRC32, const char* );
GET_FUNC( 0x65F9A0, uint32_t, CombineHashes, uint32_t, uint32_t, uint32_t );

GET_METHOD( 0x6C6D60, bool, FindSaveGameVar, TreeMap*, uint32_t, ESaveGameVar** );
GET_METHOD( 0x6C6F10, void*, AddSaveGameVar, TreeMap*, uint32_t, ESaveGameVar*, bool );

GET_METHOD( 0x6F3A50, TreeMapNode*, TreeMap_Find, TreeMap*, uint32_t, void** );

static const ETypeInfo* g_typeInfo_ESGVInt   = (const ETypeInfo*) 0x920030;
static const ETypeInfo* g_typeInfo_ESGVFloat = (const ETypeInfo*) 0x920008;


bool ESaveGameMan::GetVar( EInstance* inst, const char* valueName, int* outValue )
{
	uint32_t hash = HashStr_CRC32( valueName );
	return this->GetVar( inst, hash, outValue );
}
bool ESaveGameMan::GetVar( EInstance* inst, const char* valueName, float* outValue )
{
	uint32_t hash = HashStr_CRC32( valueName );
	return this->GetVar( inst, hash, outValue );
}
bool ESaveGameMan::GetVar( EInstance* inst, uint32_t valueNameHash, int* outValue )
{
	return ESaveGameMan_GetVar( g_typeInfo_ESGVInt, this, inst, valueNameHash, (uint32_t*) outValue, (uint32_t*) outValue ) != 0;
}
bool ESaveGameMan::GetVar( EInstance* inst, uint32_t valueNameHash, float* outValue )
{
	return ESaveGameMan_GetVar( g_typeInfo_ESGVFloat, this, inst, valueNameHash, (uint32_t*) outValue, (uint32_t*) outValue ) != 0;
}


void ESaveGameMan::SetVar( EInstance* inst, const char* valueName, int value )
{
	uint32_t hash = HashStr_CRC32( valueName );
	return this->SetVar( inst, hash, value );
}
void ESaveGameMan::SetVar( EInstance* inst, const char* valueName, float value )
{
	uint32_t hash = HashStr_CRC32( valueName );
	return this->SetVar( inst, hash, value );
}
void ESaveGameMan::SetVar( EInstance* inst, uint32_t valueNameHash, int value )
{
	ESaveGameMan_SetVar( g_typeInfo_ESGVInt, this, inst, valueNameHash, (uint32_t*) &value );
}
void ESaveGameMan::SetVar( EInstance* inst, uint32_t valueNameHash, float value )
{
	ESaveGameMan_SetVar( g_typeInfo_ESGVFloat, this, inst, valueNameHash, (uint32_t*) &value );
}


bool ESaveGameMan::GetVar( uint32_t levelCRC, uint32_t instanceID, const char* varName, int* outValue, bool imm )
{
	uint32_t varHash = HashStr_CRC32( varName );
	return GetVar( levelCRC, instanceID, varHash, outValue, imm );
}
bool ESaveGameMan::GetVar( uint32_t levelCRC, uint32_t instanceID, const char* varName, float* outValue, bool imm )
{
	uint32_t varHash = HashStr_CRC32( varName );
	return GetVar( levelCRC, instanceID, varHash, outValue, imm );
}
bool ESaveGameMan::GetVar( uint32_t levelCRC, uint32_t instanceID, uint32_t varNameHash, int* outValue, bool imm )
{
	TreeMap* saveLocation = imm ? &m_saveLocation2 : &m_saveLocation1;
	uint32_t hash = CombineHashes( levelCRC, instanceID, varNameHash );

	ESaveGameVar* sgv;
	if ( FindSaveGameVar(saveLocation, hash, &sgv) ) {
		*outValue = *((int*) &sgv->m_value);
		return true;
	}
	return false;
}
bool ESaveGameMan::GetVar( uint32_t levelCRC, uint32_t instanceID, uint32_t varNameHash, float* outValue, bool imm )
{
	TreeMap* saveLocation = imm ? &m_saveLocation2 : &m_saveLocation1;
	uint32_t hash = CombineHashes( levelCRC, instanceID, varNameHash );

	ESaveGameVar* sgv;
	if ( FindSaveGameVar(saveLocation, hash, &sgv) ) {
		*outValue = *((float*) &sgv->m_value);
		return true;
	}
	return false;
}


void ESaveGameMan::SetVar( uint32_t levelCRC, uint32_t instanceID, const char* varName, int value, bool imm )
{
	uint32_t varHash = HashStr_CRC32( varName );
	SetVar( levelCRC, instanceID, varHash, value, imm );
}
void ESaveGameMan::SetVar( uint32_t levelCRC, uint32_t instanceID, const char* varName, float value, bool imm )
{
	uint32_t varHash = HashStr_CRC32( varName );
	SetVar( levelCRC, instanceID, varHash, value, imm );
}
void ESaveGameMan::SetVar( uint32_t levelCRC, uint32_t instanceID, uint32_t varNameHash, int value, bool imm )
{
	TreeMap* saveLocation = imm ? &m_saveLocation2 : &m_saveLocation1;
	uint32_t hash = CombineHashes( levelCRC, instanceID, varNameHash );

	ESaveGameVar* sgv;
	if ( !FindSaveGameVar(saveLocation, hash, &sgv) ) {
		sgv = (ESaveGameVar*) g_typeInfo_ESGVInt->m_factory();
		AddSaveGameVar( saveLocation, hash, sgv, false );
	}

	*((int*) &sgv->m_value) = value;
}
void ESaveGameMan::SetVar( uint32_t levelCRC, uint32_t instanceID, uint32_t varNameHash, float value, bool imm )
{
	TreeMap* saveLocation = imm ? &m_saveLocation2 : &m_saveLocation1;
	uint32_t hash = CombineHashes( levelCRC, instanceID, varNameHash );

	ESaveGameVar* sgv;
	if ( !FindSaveGameVar(saveLocation, hash, &sgv) ) {
		sgv = (ESaveGameVar*) g_typeInfo_ESGVFloat->m_factory();
		AddSaveGameVar( saveLocation, hash, sgv, false );
	}

	*((float*) &sgv->m_value) = value;
}
