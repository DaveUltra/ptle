#include <Windows.h>

#include "command_palette.h"
#include "utils/log.h"
#include "injector/injector.hpp"

#include "ptle/types/types.h"


#include "ptle/EIHarry.h"
#include "utils/func.h"
GET_METHOD( 0x547180, void, EIPlayer_Tick, EIPlayer* );
void _EIPlayerTick_custom( EIHarry* harry )
{
	EIPlayer_Tick( harry );

	extern bool g_invulnerable;
	if ( g_invulnerable ) {
		bool* inv = (bool*) (0x917059);
		*inv = true;
	}
	extern bool g_infiniteJumps;
	if ( g_infiniteJumps ) {
		harry->m_onGround = true;
	}
}
MAKE_THISCALL_WRAPPER( EIPlayerTick_custom, _EIPlayerTick_custom );

#include <unordered_set>
#include "ptle/EScriptContext.h"
#include "ptle/ERScript.h"
#include "ptle/ESDInt.h"
#include "ptle/ESDFloat.h"
#include "ptle/ESDVector.h"
#include "ptle/ESDMatrix.h"
#include "ptle/ESDString.h"
#include "ptle/ESDResource.h"
struct ScriptManager
{
	ETypeInfo* m_scriptDataTypes[256];
	ScriptFunction* m_funcLibsPointers[32];
	int m_funcLibsNumFuncs[32];
	int m_funcLibsNum;
	TreeMap idk0;
	TreeMap m_globalScriptDatas;
	TreeMap idk2;
	TreeMap idk3;
	// ...

	virtual ~ScriptManager();
};
static_assert( (uint32_t) &reinterpret_cast<ScriptManager*>(0)->m_globalScriptDatas == 0x514, "Field offset mismatch" );
GET_METHOD( 0x6C3020, bool, EScriptContext_ExecuteScript, EScriptContext* );
GET_METHOD( 0x659EA0, ScriptFunction*, GetFunctionFromBytecode, ScriptManager*, uint32_t );
GET_METHOD( 0x6F3A50, TreeMapNode*, TreeMap_Find, TreeMap*, uint32_t, void** );
void displayScriptData(EScriptData* s)
{
	std::string typeName = s->GetTypeName();
	log_printf("%s, ", typeName.c_str());
	if (typeName == "ESDInt") {
		log_printf("%d (0x%X)", ((ESDInt*) s)->m_value, ((ESDInt*) s)->m_value);
	}
	else if (typeName == "ESDFloat") {
		log_printf("%f", ((ESDFloat*) s)->m_value);
	}
	else if (typeName == "ESDVector") {
		Vector3f& v = ((ESDVector*) s)->m_value;
		log_printf("%f, %f, %f", v.x, v.y, v.z);
	}
	else if (typeName == "ESDMatrix") {

	}
	else if (typeName == "ESDString") {
		log_printf( ((ESDString*) s)->m_value );
	}
	else if (is_type_derived_from(s->GetType(), (ETypeInfo*) 0x91FF2C)) {
		/*if (((ESDResource*)s)->m_name) {
			log_printf("%s", ((ESDResource*) s)->m_name);
		}
		if (((ESDResource*) s)->m_resource) {
			log_printf(" (%s)", ((ESDResource*) s)->m_resource->m_name);
		}*/
	}
}

bool g_logScripts = false;
static std::unordered_set<std::string> m;
void LogScript( EScriptContext* ctx )
{
	// Gate to execute once.
	if ( m.find(ctx->m_script->m_name) != m.end() ) return;
	m.insert(ctx->m_script->m_name);

	log_printf( "Executing \"%s\"...\n", ctx->m_script->m_name );

	ScriptManager* global = (ScriptManager*) 0x91F918;

	log_printf("- Data :\n");
	for ( TLinkedListElem<EScriptData*>* e = ctx->m_script->m_scriptData.m_iterateFirst; e != 0; e = e->m_next ) {
		EScriptData* data = e->m_data;
		log_printf("  - ");
		displayScriptData( data );
		log_printf("\n");
	}

	log_printf("- Bytecode :\n");
	uint32_t* bytecode = (uint32_t*) ctx->m_script->m_bytecode.m_data;
	for ( int i = 0; i < ctx->m_script->m_bytecode.m_size; i++ ) {
		log_printf("  - ");

		uint32_t code = bytecode[i];
		switch (code & 0xF)
		{
		case 0:
		case 4:
		case 6:
			log_printf("unknown action (0x%X)\n", code);
			break;

		case 1:
			log_printf("repush %d to last (", code >> 4);
			//displayScriptData( ctx->m_scriptStack[sp - (code >> 4)] );
			log_printf(")\n");
			break;
		case 2:
			log_printf("create and push (%s)\n", global->m_scriptDataTypes[code >> 4]->m_typeName);
			break;
		case 3: {
				i++;
				uint32_t code2 = bytecode[i];
				log_printf("push global (%c, %x)", (char) ((code >> 4) >> 0x10), ((code2 >> 4) << 10) | ((code >> 4) & 0xFFFF));
				EScriptData* out;
				if ( TreeMap_Find(&global->m_globalScriptDatas, ((code2 >> 4) << 0x10) | ((code >> 4) & 0xFFFF), (void**) &out) ) {
					log_printf("  -  ");
					displayScriptData( out );
				}
				log_printf("\n");
			}
			break;
		case 5:
			log_printf("pop %d\n", code >> 4);
			break;
		case 7:
			log_printf("set bytecode pointer (%d)\n", (code >> 4) & 0xFFFFF);
			break;
		case 8: {
				ScriptFunction* func = GetFunctionFromBytecode( global, code >> 4 );
				if ( func ) {
					log_printf("%s(%s) (0x%X)\n", func->m_functionName, func->m_paramTypes, func->m_functionPtr);
				}
				else {
					log_printf("function not found (0x%X)\n", code);
				}
			}
			break;
		}
	}
	log_printf("\n");
}

GET_FUNC( 0x65CC10, char**, PopGetString, EScriptContext* );
void Script_Print( EScriptContext* ctx )
{
	char** str = PopGetString( ctx );
	//log_printf("%s : %s\n", ctx->m_script->m_name, *str);
}



unsigned long __stdcall Thread( void* )
{
	command_create_window();
	command_process_window();

	ExitProcess( 0 );
}


#include "gizmod/Gizmod.h"
#include "gizmod/GizmodPlugin.h"

#include "gizmod/event/ExecuteScriptEvent.h"

class CheatLabPlugin : public GizmodPlugin, public IExecuteScriptListener
{
public:

	virtual const char* getDisplayName() const { return "CheatLab"; }
	virtual const char* getSystemName() const { return "CheatLab"; }

	virtual void onEnable()
	{
		injector::MakeCALL( 0x4B2DE1, EIPlayerTick_custom );

		Gizmod::getInstance()->getEventListener()->registerEvent<ExecuteScriptEvent>( this );

		//injector::WriteMemory( 0x8FBB1C, &Script_Print );

		register_types();

		CreateThread( 0, 0, Thread, 0, 0, 0 );
	}

	virtual void onExecuteScript( ExecuteScriptEvent& event )
	{
		if ( !g_logScripts ) return;
		LogScript( event.getScriptContext() );
	}
};

DECLARE_PLUGIN( CheatLabPlugin );
