#pragma once

#include <stdint.h>

class EStorable;


class ETypeInfo
{
public:

	EStorable* (*m_factory)();
	EStorable* (*m_constructor)( EStorable* );
	void       (*m_destructor)( EStorable* );
	const char*  m_typeName;
	uint32_t     m_hash;
	uint32_t     unknown;
	ETypeInfo*   m_parent;   // Base type.
	ETypeInfo*   m_left;     // Binary tree structure.
	ETypeInfo*   m_right;    //
};

struct type_info_t
{
	void* vtable;
	const ETypeInfo* ptleType;

	inline const char* get_name() const
	{
		return (ptleType != 0) ? ptleType->m_typeName : 0;
	}
};


const type_info_t* get_type_by_vtable( uint32_t vtableAddr );

void register_types();
