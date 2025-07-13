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



template<typename T>
T* instantiate_object( const type_info_t* t )
{
	return reinterpret_cast<T*>( t->ptleType->m_factory() );
}

const type_info_t* get_object_type( EStorable* obj );

const type_info_t* get_type_by_vtable( uint32_t vtableAddr );

bool is_type_derived_from( const type_info_t* type, const type_info_t* base );

bool is_type_derived_from( const ETypeInfo* type, const ETypeInfo* base );

template<typename T>
T* type_cast( EStorable* obj, const type_info_t* t )
{
	const type_info_t* objType = get_object_type( obj );
	if ( !objType || !is_type_derived_from(objType, t) ) {
		return 0;
	}

	return reinterpret_cast<T*>(obj);
};


void register_types();
