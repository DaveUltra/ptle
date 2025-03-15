/*
 * JSON Parser.
 * Internal : This should not be included by the user.
 */

#pragma once

#include <string>


namespace json
{

class JSONDocument;
class JSONObject;
class JSONArray;
class JSONValue;

class JSONParser
{
public:

	JSONParser() { }

	JSONObject* Parse( JSONDocument* doc, const std::string& jsonData );

private:

	size_t SkipWhitespace();
	size_t SkipUntilNotKeyword();
	size_t Find( char c, size_t start = -1 ) const;
	size_t FindOneOf( int n, char* b ) const;
	size_t FindNotKeyword( size_t start = -1 ) const;

		/// Creates a substring from the source JSON. End index is exclusive.
	std::string Substring( int b, int e ) const;

	JSONObject* ParseObject();
	JSONArray* ParseArray();
	JSONValue* ParseValue();


private:

	std::string buffer;
	size_t pointer;
};

}
