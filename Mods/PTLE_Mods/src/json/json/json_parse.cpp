#include "json.h"
#include "json_parse.h"


namespace json
{

JSONObject* JSONParser::Parse( JSONDocument* doc, const std::string& jsonData )
{
	buffer = jsonData;
	pointer = 0;

	SkipWhitespace();
	if ( pointer == -1 || buffer[pointer] != '{' ) return 0;
	pointer++;

	return ParseObject();
}

size_t JSONParser::SkipWhitespace()
{
	char c;
	c = buffer[pointer];
	while ( c != 0 && isspace(c) ) {
		pointer++;
		c = buffer[pointer];
	}

	return c == 0 ? -1 : pointer;
}

size_t JSONParser::SkipUntilNotKeyword()
{
	return -1;
}

size_t JSONParser::Find( char ch, size_t start ) const
{
	size_t p = start == -1 ? pointer : start;
	char c;

	c = buffer[p];
	while ( c != 0 ) {
		if ( c == ch ) {
			return p;
		}
		p++;
		c = buffer[p];
	}
	return -1;
}

size_t JSONParser::FindOneOf( int n, char* b ) const
{
	size_t p = pointer;
	char c;

	c = buffer[p];
	while ( c != 0 ) {
		for ( char* pc = b; pc != b + n; pc++ ) {
			if ( c == *pc ) {
				return p;
			}
		}
		p++;
		c = buffer[p];
	}
	return -1;
}

size_t JSONParser::FindNotKeyword( size_t start ) const
{
	size_t p = start == -1 ? pointer : start;
	char c;

	c = buffer[p];
	while ( c != 0 ) {
		if ( c < 'a' || c > 'z' ) {
			return p;
		}
		p++;
		c = buffer[p];
	}
	return -1;
}

std::string JSONParser::Substring( int b, int e ) const
{
	return buffer.substr( b, e - b );
}


JSONObject* JSONParser::ParseObject()
{
	JSONObject* obj = new JSONObject();

	size_t p0, p1;
	while ( 1 ) {
		// Field name.
		p0 = Find( '\"' );
		p1 = Find( '\"', p0 + 1 );
		std::string fieldName = Substring( p0 + 1, p1 );

		p0 = Find( ':', p1 + 1 );
		pointer = p0 + 1;

		// Field value.
		SkipWhitespace();
		JSONValue* value = ParseValue();
		if ( value ) {
			obj->_AddInternal( fieldName, value );
		}

		pointer = FindOneOf( 2, ",}" );
		if ( buffer[pointer] == '}' ) {
			pointer++;
			break;
		}

		pointer++;
	}

	return obj;
}

JSONArray* JSONParser::ParseArray()
{
	JSONArray* arr = new JSONArray();
	JSONValue *first = 0, *last = 0;

	while ( 1 ) {
		SkipWhitespace();

		// Array end.
		if ( buffer[pointer] == ']' ) {
			pointer++;
			break;
		}

		JSONValue* val = ParseValue();
		if ( !first ) {
			first = val;
		}
		if ( last ) {
			last->next = val;
		}
		last = val;

		SkipWhitespace();
		if ( buffer[pointer] == ',' ) {
			pointer++;
		}
	}

	arr->first = first;
	return arr;
}

JSONValue* JSONParser::ParseValue()
{
	JSONType valueType = JSON_UNDEFINED;
	JSONDataVariant variant;
	std::string token;

	variant.m_object = 0;

	size_t p0, p1;
	p0 = pointer;
	char firstChar = buffer[pointer];
	switch ( firstChar )
	{
	// Object.
	case '{':
		pointer++;
		variant.m_object = ParseObject();
		if ( variant.m_object ) {
			valueType = json::JSON_OBJECT;
		}
		break;

	// Array.
	case '[':
		pointer++;
		variant.m_array = ParseArray();
		if ( variant.m_array ) {
			valueType = json::JSON_ARRAY;
		}
		break;

	// String.
	case '\"':
		p1 = Find( '\"', p0 + 1 );
		token = Substring( p0 + 1, p1 );
		variant.m_string = (char*) malloc( token.size() + 1 );
		variant.m_stringLength = token.size();
		memcpy( (char*) variant.m_string, token.c_str(), token.size() + 1 );
		valueType = json::JSON_STRING;
		pointer = p1 + 1;
		break;

	// Certainly a keyword.
	case 't':
	case 'f':
	case 'n':
		p1 = FindNotKeyword( p0 + 1 );
		token = Substring( p0, p1 );
		if ( token == "true" ) {
			valueType = json::JSON_BOOL;
			variant.m_bool = true;
		}
		else if ( token == "false" ) {
			valueType = json::JSON_BOOL;
			variant.m_bool = false;
		}
		else if ( token == "null" ) {
			valueType = json::JSON_NULL;
		}
		else {

		}
		pointer = p1;
		break;

	// Decimal number.
	case '-':
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		break;

	// Unknown.
	default:
		break;
	}

	if ( valueType == json::JSON_UNDEFINED ) {
		return 0;
	}

	JSONValue* value = new JSONValue();
	value->type = valueType;
	value->variant = variant;

	return value;
}


} // namespace json.
