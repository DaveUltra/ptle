/*
 * Custom JSON parser for the Pitfall repo.
 *   by David Kroft
 *   inspired by tinyxml2
 *
 * Current flaws :
 * - The parser does not understand number types (ints, floats) and might
 *   crash / loop indefinitely when encountering one.
 * - JSON elements are all allocated using malloc(). This is inefficient,
 *   allocations should be pooled in the future.
 */


#pragma once

#include <string>
#include <map>

#include <stdint.h>
#include <assert.h>


#define JSON_ASSERT( expr ) assert( expr )
#define JSON_ASSERT2( expr, msg ) (void)( (!!(expr)) || (_wassert(L ## msg, _CRT_WIDE(__FILE__), __LINE__), 0) )


namespace json
{

class JSONArray;
class JSONObject;


/// Value types.
enum JSONType
{
	JSON_UNDEFINED,
	JSON_NULL,
	JSON_BOOL,
	JSON_INT,
	JSON_FLOAT,
	JSON_STRING,
	JSON_ARRAY,
	JSON_OBJECT,
};


/// JSON value storage.
struct JSONDataVariant
{
	union {
		bool m_bool;
		int64_t m_int;
		float m_float;
		struct {
			const char* m_string;
			size_t m_stringLength;
		};
		JSONArray* m_array;
		JSONObject* m_object;
	};
};


/// JSON "value".
/// A value is any bit of data expressible by itself, without a name.
/// This can be a primitive such as bool, int or float, or compound types
/// like arrays and objects. It can also be null.
/// JSONField extends this class by giving each value a name string.
class JSONValue
{
	friend class JSONParser;
	friend class JSONArray;

public:

	JSONValue() : type(JSON_UNDEFINED), next(0) { }
	~JSONValue();

	static const JSONValue& Undefined() { static JSONValue g_undef; return g_undef; }

	bool IsNull() const { return type == JSON_NULL; }
	bool IsUndefined() const { return this == &JSONValue::Undefined(); }


	bool GetBoolValue( bool defaultValue = false ) const;
	int GetIntValue( int defaultValue = 0 ) const;
	float GetFloatValue( float defaultValue = 0.0F ) const;
	std::string GetStringValue( const std::string& defaultValue = "" ) const;
	JSONArray& GetArrayValue() const;
	JSONObject& GetObjectValue() const;

	void SetNullValue();
	void SetBoolValue( bool value );
	void SetIntValue( int value );
	void SetFloatValue( float value );
	void SetStringValue( const std::string& value );
	JSONArray* SetArrayValue();
	JSONArray* SetArrayValue( JSONArray* arr );
	JSONObject* SetObjectValue();
	JSONObject* SetObjectValue( JSONObject* obj );

	std::string ToString() const;


	JSONValue* NextArrayValue() const;

	JSONType Type() const { return type; }

private:

	void DestroyData();

protected:

	JSONDataVariant variant;
	JSONType type;
	JSONValue* next;  // Linked list for arrays and object fields.
};


/// JSON field.
/// This represents a single named element inside an object.
/// Beware, as this can hold an undefined value.
class JSONField
{
	friend class JSONObject;

public:

	JSONField( const std::string& name ) : name(name), value((JSONValue*) &JSONValue::Undefined()) { }
	~JSONField();

	//JSONField* NextField() const;

	bool IsNull() const { return value->IsNull(); }
	bool IsUndefined() const { return value->IsUndefined(); }

	const std::string& Name() const { return name; }
	JSONType Type() const { return value->Type(); }


		/// Developer convenience.
	operator std::string() { return value->ToString(); }
	operator JSONValue() { return *value; }
	JSONValue* operator->() { return value; }


private:

	std::string name;
	JSONValue* value;
};


/// JSON array.
class JSONArray
{
	friend class JSONParser;

public:

	struct iterator
	{
		iterator( JSONValue* ptr ) : ptr(ptr) {}

		JSONValue& operator*() {
			return *ptr;
		}

		bool operator!=( const iterator& other ) const {
			return ptr != other.ptr;
		}

		void operator++() {
			ptr = ptr->NextArrayValue();
		}

		JSONValue* ptr;
	};

public:

	JSONArray() : first( 0 ) { }
	~JSONArray();

	bool IsEmpty() const { return true; }

	JSONValue* FirstValue() const { return first; }


	iterator begin() { return iterator(FirstValue()); }
	iterator end() { return iterator(0); }

private:

	JSONValue* first;
};


/// JSON object.
class JSONObject
{
public:

	typedef std::map<std::string, JSONField*> field_map;

	struct iterator : field_map::iterator
	{
		// Upcast constructor.
		iterator( const field_map::iterator& o ) : field_map::iterator(o) { }

		JSONField& operator*() { return *this->operator->()->second; }
	};

public:

	~JSONObject();

	bool IsEmpty() const { return fields.empty(); }

	JSONField* FirstField() const;
	int NumFields() const { return fields.size(); }

	JSONField* FindField( const std::string& name ) const;

	void _AddInternal( const std::string& name, JSONValue* value );
	void Remove( const std::string& name );

	void SetBool( const std::string& name, bool value );
	void SetInt( const std::string& name, int value );
	void SetFloat( const std::string& name, float value );
	void SetString( const std::string& name, const std::string& value );
	void SetNull( const std::string& name );
	JSONArray* SetArray( const std::string& name );
	JSONArray* SetArray( const std::string& name, JSONArray* arr );
	JSONObject* SetObject( const std::string& name );
	JSONObject* SetObject( const std::string& name, JSONObject* obj );


	JSONField& operator[]( const std::string& name ) const;


	iterator begin() { return fields.begin(); }
	iterator end() { return fields.end(); }

private:

	JSONField* FindFieldInsert( const std::string& name );


private:

	field_map fields;
};


/// Root document.
class JSONDocument
{
public:

	JSONDocument();
	~JSONDocument();

	bool IsValid() const { return m_rootObject != 0; }

	void LoadFile( const char* path );

	JSONObject& Root() { return *m_rootObject; }


private:

	JSONObject* m_rootObject;
};


} // namespace json
