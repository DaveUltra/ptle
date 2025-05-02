#include "rando/rando.h"

#include "ptle/levels/level_info.h"

#include "json/json.h"
#include "utils/log.h"


MajorAreas transition_infos;

std::map<uint32_t, Area*> level_infos;


static uint32_t parse_int_hex( const std::string& str )
{
	uint32_t v = 0;
	int i = 0;

	// Skip prefix.
	if ( str[0] == '0' && (str[1] == 'x' || str[1] == 'X') ) {
		i = 2;
	}

	while ( i != str.size() ) {
		v *= 16;

		char c = str[i];
		if ( c >= '0' && c <= '9' ) {
			v += (uint32_t) (c - '0');
		}
		else if ( c >= 'A' && c <= 'F' ) {
			v += (uint32_t) (c - 'A' + 10);
		}
		else if ( c >= 'a' && c <= 'f' ) {
			v += (uint32_t) (c - 'a' + 10);
		}

		i++;
	}

	return v;
}

bool load_transition_infos()
{
	json::JSONDocument doc;
	doc.LoadFile( "cfg/Randomizer/transition_infos.json" );

	if ( !doc.IsValid() ) {
		log_printf( "ERROR : transition_infos.json not found!\n" );
		return false;
	}

	json::JSONObject& root = doc.Root();

	// Major areas (jungle, mountains, native territory, etc...).
	for ( json::JSONField& majorArea : root ) {
		// Filter out.
		if ( majorArea.Name() == "//" ) continue;
		if ( majorArea.Type() != json::JSON_ARRAY ) continue;

		auto it = transition_infos.insert( MajorAreas::value_type(majorArea.Name(), std::vector<Area*>()) ).first;
		std::vector<Area*>& areas = it->second;

		// Areas (or levels, if you're like me).
		for ( json::JSONValue& area : majorArea->GetArrayValue() ) {
			if ( area.Type() != json::JSON_OBJECT ) continue;
			json::JSONObject& areaObj = area.GetObjectValue();

			Area* a = new Area( parse_int_hex(areaObj["area_id"]), areaObj["area_name"], parse_int_hex(areaObj["default_entrance"]) );
			areas.push_back( a );
			level_infos.emplace( level_get_by_crc(a->areaCRC), a );

			// Area exits.
			json::JSONField& exits = areaObj["exits"];
			if ( exits.IsUndefined() || exits.Type() != json::JSON_ARRAY ) continue;

			for ( json::JSONValue& exit : exits->GetArrayValue() ) {
				if ( exit.Type() != json::JSON_OBJECT ) continue;
				json::JSONObject& exitObj = exit.GetObjectValue();

				a->exits.push_back( Exit( parse_int_hex(exitObj["area_id"]), exitObj["area_name"] ) );
				Exit& e = a->exits.back();

				// Exit requirements.
				json::JSONField& requires = exitObj["requires"];
				if ( requires.IsUndefined() || requires.Type() != json::JSON_ARRAY ) continue;
				if ( requires->GetArrayValue().IsEmpty() ) continue;

				for ( json::JSONValue& req : requires->GetArrayValue() ) {
					if ( req.Type() == json::JSON_ARRAY ) {
						e.requires.push_back( std::vector<std::string>() );
						std::vector<std::string>& reqlist = e.requires.back();
						for ( json::JSONValue& partreq : req.GetArrayValue() ) {
							if ( partreq.Type() != json::JSON_STRING ) continue;
							reqlist.push_back( partreq.GetStringValue() );
						}
					}
					else if ( req.Type() == json::JSON_STRING ) {
						if ( e.requires.empty() ) {
							e.requires.push_back( std::vector<std::string>() );
						}
						std::vector<std::string>& reqlist = e.requires[0];
						reqlist.push_back( req.GetStringValue() );
					}
				}
			}
		}
	}

	return true;
}
