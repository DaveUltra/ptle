#include "rando/rando.h"

#include "json/json.h"
#include "utils/log.h"


IdolsList idols_info;
ExplorerList explorer_info;
ShamanList shaman_info;


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

bool load_idols_infos()
{
	json::JSONDocument doc;
	doc.LoadFile( "config/Randomizer/idols_infos.json" );

	if ( !doc.IsValid() ) {
		log_printf( "ERROR : idols_infos.json not found!\n" );
		return false;
	}

	json::JSONObject& root = doc.Root();

	// Major areas (jungle, mountains, native territory, etc...).
	for ( json::JSONField& majorArea : root ) {
		// Filter out.
		if ( majorArea.Name() == "//" ) continue;
		if ( majorArea.Type() != json::JSON_ARRAY ) continue;

		// Areas (or levels, if you're like me).
		for ( json::JSONValue& area : majorArea->GetArrayValue() ) {
			if ( area.Type() != json::JSON_OBJECT ) continue;
			json::JSONObject& areaObj = area.GetObjectValue();

			const uint32_t areaCRC = parse_int_hex(areaObj["area_id"]);
			std::vector<Idol>& idolsByArea = idols_info.emplace( areaCRC, std::vector<Idol>() ).first->second;

			// Shaman.
			json::JSONField& shaman = areaObj["shaman"];
			if ( !shaman.IsUndefined() && shaman.Type() == json::JSON_OBJECT ) {
				json::JSONObject& shamanObj = shaman->GetObjectValue();

				ShamanInfo shamanInfo = { areaCRC };
				shaman_info.emplace( areaCRC, shamanInfo );
			}

			// Idols.
			json::JSONField& idols = areaObj["idols"];
			if ( idols.IsUndefined() || idols.Type() != json::JSON_ARRAY ) continue;

			for ( json::JSONValue& idol : idols->GetArrayValue() ) {
				if ( idol.Type() != json::JSON_OBJECT ) continue;
				json::JSONObject& idolObj = idol.GetObjectValue();

				// Idol data.
				Idol data = { areaCRC, 0, false };
				if ( !idolObj["duplicate"].IsUndefined() ) {
					data.m_duplicate = (idolObj["duplicate"].operator std::string() == "true");
				}

				if ( idolObj["explorer"].IsUndefined() ) {
					data.m_uniqueID = parse_int_hex(idolObj["unique_id"]);
					idolsByArea.push_back( data );
				}
				else {
					data.initAsExplorer();
					explorer_info.emplace( areaCRC, data );
				}

				// Requirements.
				/*json::JSONField& requires = exitObj["requires"];
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
				}*/
			}
		}
	}

	return true;
}
