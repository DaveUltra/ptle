#include "json.h"

#include "json_parse.h"


namespace json
{

// JSON value.

JSONValue::~JSONValue()
{
	DestroyData();
}

bool JSONValue::GetBoolValue( bool defaultValue ) const
{
	return type == JSON_BOOL ? variant.m_bool : defaultValue;
}

int JSONValue::GetIntValue( int defaultValue ) const
{
	return type == JSON_INT ? variant.m_int : defaultValue;
}

float JSONValue::GetFloatValue( float defaultValue ) const
{
	return type == JSON_FLOAT ? variant.m_float : defaultValue;
}

std::string JSONValue::GetStringValue( const std::string& defaultValue ) const
{
	return type == JSON_STRING ? variant.m_string : defaultValue;
}

JSONArray& JSONValue::GetArrayValue() const
{
	return *variant.m_array;
}

JSONObject& JSONValue::GetObjectValue() const
{
	return *variant.m_object;
}

void JSONValue::SetNullValue()
{
	JSON_ASSERT2( !this->IsUndefined(), "Cannot modify undefined" );
	this->DestroyData();
	type = JSON_NULL;
}

void JSONValue::SetBoolValue( bool value )
{
	JSON_ASSERT2( !this->IsUndefined(), "Cannot modify undefined" );
	this->DestroyData();
	type = JSON_BOOL;
	variant.m_bool = value;
}

void JSONValue::SetIntValue( int value )
{
	JSON_ASSERT2( !this->IsUndefined(), "Cannot modify undefined" );
	this->DestroyData();
	type = JSON_INT;
	variant.m_int = value;
}

void JSONValue::SetFloatValue( float value )
{
	JSON_ASSERT2( !this->IsUndefined(), "Cannot modify undefined" );
	this->DestroyData();
	type = JSON_FLOAT;
	variant.m_float = value;
}

void JSONValue::SetStringValue( const std::string& value )
{
	JSON_ASSERT2( !this->IsUndefined(), "Cannot modify undefined" );
	this->DestroyData();
	type = JSON_STRING;
	variant.m_string = (char*) malloc( value.size() + 1 );
	variant.m_stringLength = value.size();
	memcpy( (char*) variant.m_string, value.c_str(), value.size() + 1 );
}

JSONArray* JSONValue::SetArrayValue()
{
	JSON_ASSERT2( !this->IsUndefined(), "Cannot modify undefined" );
	this->DestroyData();
	type = JSON_ARRAY;
	variant.m_array = new JSONArray();
	return variant.m_array;
}

JSONArray* JSONValue::SetArrayValue( JSONArray* arr )
{
	JSON_ASSERT2( !this->IsUndefined(), "Cannot modify undefined" );
	this->DestroyData();
	type = JSON_ARRAY;
	variant.m_array = arr;
	return arr;
}

JSONObject* JSONValue::SetObjectValue()
{
	JSON_ASSERT2( !this->IsUndefined(), "Cannot modify undefined" );
	this->DestroyData();
	type = JSON_OBJECT;
	variant.m_object = new JSONObject();
	return variant.m_object;
}

JSONObject* JSONValue::SetObjectValue( JSONObject* obj )
{
	JSON_ASSERT2( !this->IsUndefined(), "Cannot modify undefined" );
	this->DestroyData();
	type = JSON_OBJECT;
	variant.m_object = obj;
	return obj;
}

std::string JSONValue::ToString() const
{
	switch ( type )
	{
	case JSON_UNDEFINED: return "undefined";
	case JSON_NULL: return "null";
	case JSON_BOOL: return variant.m_bool ? "true" : "false";
	case JSON_INT: return std::to_string( variant.m_int );
	case JSON_FLOAT: return std::to_string( variant.m_float );
	case JSON_STRING: return variant.m_string;
	default:
		return "";
	}
}

JSONValue* JSONValue::NextArrayValue() const
{
	return next;
}

void JSONValue::DestroyData()
{
	switch ( type )
	{
	case JSON_STRING: free( (char*) variant.m_string ); break;
	case JSON_ARRAY: delete variant.m_array; break;
	case JSON_OBJECT: delete variant.m_object; break;
	}
}


// JSON field.

JSONField::~JSONField()
{
	if ( !value->IsUndefined() ) {
		delete value;
	}
}


// JSON array.

JSONArray::~JSONArray()
{
	JSONValue* next;
	while ( first ) {
		next = first->next;
		delete first;
		first = next;
	}
	first = 0;
}


// JSON object.

JSONObject::~JSONObject()
{
	for ( auto& p : fields ) {
		delete p.second;
	}
}

JSONField* JSONObject::FindField( const std::string& name ) const
{
	static JSONField g_undefField( "undefined" );

	auto it = fields.find( name );
	if ( it == fields.end() ) return &g_undefField;

	return it->second;
}

void JSONObject::_AddInternal( const std::string& name, JSONValue* value )
{
	JSONField* field = new JSONField( name );
	field->value = value;
	fields.insert( field_map::value_type(name, field) );
}

void JSONObject::Remove( const std::string& name )
{
	fields.erase( name );
}

void JSONObject::SetBool( const std::string& name, bool value )
{
	JSONField& field = *this->FindFieldInsert( name );
	field->SetBoolValue( value );
}

void JSONObject::SetInt( const std::string& name, int value )
{
	JSONField& field = *this->FindFieldInsert( name );
	field->SetIntValue( value );
}

void JSONObject::SetFloat( const std::string& name, float value )
{
	JSONField& field = *this->FindFieldInsert( name );
	field->SetFloatValue( value );
}

void JSONObject::SetString( const std::string& name, const std::string& value )
{
	JSONField& field = *this->FindFieldInsert( name );
	field->SetStringValue( value );
}

void JSONObject::SetNull( const std::string& name )
{
	JSONField& field = *this->FindFieldInsert( name );
	field->SetNullValue();
}

JSONArray* JSONObject::SetArray( const std::string& name )
{
	JSONField& field = *this->FindFieldInsert( name );
	return field->SetArrayValue();
}

JSONArray* JSONObject::SetArray( const std::string& name, JSONArray* arr )
{
	JSONField& field = *this->FindFieldInsert( name );
	return field->SetArrayValue( arr );
}

JSONObject* JSONObject::SetObject( const std::string& name )
{
	JSONField& field = *this->FindFieldInsert( name );
	return field->SetObjectValue();
}

JSONObject* JSONObject::SetObject( const std::string& name, JSONObject* obj )
{
	JSONField& field = *this->FindFieldInsert( name );
	return field->SetObjectValue( obj );
}

JSONField& JSONObject::operator[]( const std::string& name ) const
{
	return *this->FindField( name );
}

JSONField* JSONObject::FindFieldInsert( const std::string& name )
{
	auto it = fields.find( name );
	if ( it == fields.end() ) {
		JSONField* field = new JSONField( name );
		it = fields.insert(field_map::value_type(name, field)).first;
	}
	return it->second;
}


// JSON document.

JSONDocument::JSONDocument()
	: m_rootObject( 0 )
{

}

JSONDocument::~JSONDocument()
{
	if ( m_rootObject ) {
		delete m_rootObject;
	}
}

void JSONDocument::LoadFile( const char* path )
{
	if ( m_rootObject ) {
		delete m_rootObject;
		m_rootObject = 0;
	}

	FILE* f = fopen( path, "rb" );
	if ( !f ) {
		return;
	}

	fseek( f, 0, SEEK_END );
	size_t size = ftell( f );
	fseek( f, 0, SEEK_SET );
	char* jsonData = (char*) malloc( size + 1 );
	fread( jsonData, size, 1, f );
	jsonData[size] = 0;

	JSONParser parser;
	m_rootObject = parser.Parse( this, jsonData );

	free( jsonData );
	fclose( f );
}

} // namespace json
