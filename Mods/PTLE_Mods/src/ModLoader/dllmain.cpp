#include "dllmain.h"
#include "exception.hpp"
#include <initguid.h>
#include <filesystem>
#include <fstream>
#include <memory>
#include <shellapi.h>
#include <Commctrl.h>
#pragma comment(lib, "delayimp")
#pragma comment(lib,"Comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define _WIDEN(x) L ## x
#define WIDEN(x) _WIDEN(x)

const int DEFAULT_BUTTON = 1000;


extern "C" IMAGE_DOS_HEADER __ImageBase;
typedef NTSTATUS(NTAPI* LdrAddRefDll_t)(ULONG, HMODULE);



#define TARGET_DLL_BINKW32


//
// Pitfall Mod Loader instance.
//

#include "gizmod/Gizmod.h"
#include "gizmod/GizmodPlugin.h"

extern "C" { Gizmod* gizmodInstance; }

bool g_enabled = true;
bool g_skipSplashScreens = false;

Gizmod g_pitfall;



const int countdownSeconds = 10;
HRESULT CALLBACK TaskDialogCallbackProc(HWND hwnd, UINT uNotification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData)
{
	static bool userInteracted = false;
	static UINT_PTR timerID = 1;
	static int remainingSeconds = countdownSeconds;
	static auto mainDialogHwnd = hwnd;

	switch (uNotification)
	{
	case TDN_CREATED:
	{
		// Initialize progress bar
		SendMessage(hwnd, TDM_SET_PROGRESS_BAR_RANGE, 0, MAKELPARAM(0, countdownSeconds));
		SendMessage(hwnd, TDM_SET_PROGRESS_BAR_POS, countdownSeconds, 0);

		// Set timer for countdown (1000ms = 1 second)
		timerID = SetTimer(mainDialogHwnd, timerID, 1000, NULL);

		// Create a hook to capture ALL window messages for this thread
		// This is more reliable than window subclassing for complex dialogs
		SetWindowsHookEx(WH_GETMESSAGE, [](int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT
		{
			if (nCode >= 0 && wParam == PM_REMOVE)
			{
				MSG* msg = (MSG*)lParam;
				switch (msg->message)
				{
				case WM_MOUSEMOVE:
				case WM_KEYDOWN:
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_MBUTTONDOWN:
				case WM_NCHITTEST:
				case WM_SETCURSOR:
					if (!userInteracted)
					{
						if (msg->message == WM_MOUSEMOVE && remainingSeconds >= countdownSeconds - 1)
							break;

						userInteracted = true;
						KillTimer(mainDialogHwnd, timerID);
						SendMessage(mainDialogHwnd, TDM_SET_PROGRESS_BAR_POS, 0, 0);
					}
					break;
				}
			}
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		}, NULL, GetCurrentThreadId());
	}
	break;
	case TDN_TIMER:
		if (remainingSeconds > 0 && !userInteracted)
		{
			remainingSeconds--;
			SendMessage(hwnd, TDM_SET_PROGRESS_BAR_POS, remainingSeconds, 0);

			std::wstring progressText = L"Auto-closing in " + std::to_wstring(remainingSeconds) + L" seconds...";
			SendMessage(hwnd, TDM_SET_ELEMENT_TEXT, TDE_FOOTER, (LPARAM)progressText.c_str());

			if (remainingSeconds == 0)
			{
				KillTimer(hwnd, timerID);
				SendMessage(hwnd, TDM_CLICK_BUTTON, DEFAULT_BUTTON, 0);
			}
		}
	break;
	case TDN_BUTTON_CLICKED:
		if (!userInteracted) {
			userInteracted = true;
			KillTimer(hwnd, timerID);
			SendMessage(hwnd, TDM_SET_PROGRESS_BAR_POS, 0, 0);
		}
	break;
	}

	return S_OK;
}

HMODULE hm;
std::wstring moduleFileName;


static void wstr_to_lower(std::wstring& s)
{
	for ( wchar_t& c : s ) {
		c = towlower( c );
	}
}

static bool iequals(const std::wstring& s1, const std::wstring& s2)
{
	std::wstring str1(s1);
	std::wstring str2(s2);
	wstr_to_lower( str1 );
	wstr_to_lower( str2 );
	return (str1 == str2);
}


std::wstring to_wstring(const std::string& cstr)
{
	std::string str(std::move(cstr));
	auto charsReturned = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(charsReturned, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], charsReturned);
	return wstrTo;
}

std::wstring SHGetKnownFolderPath(REFKNOWNFOLDERID rfid, DWORD dwFlags, HANDLE hToken)
{
	std::wstring r;
	WCHAR* szSystemPath = nullptr;
	if (SUCCEEDED(SHGetKnownFolderPath(rfid, dwFlags, hToken, &szSystemPath)))
	{
		r = szSystemPath;
	}
	CoTaskMemFree(szSystemPath);
	return r;
};

std::wstring GetCurrentDirectoryW()
{
	static const auto INITIAL_BUFFER_SIZE = MAX_PATH;
	static const auto MAX_ITERATIONS = 7;
	std::wstring ret;
	DWORD bufferSize = INITIAL_BUFFER_SIZE;
	for (size_t iterations = 0; iterations < MAX_ITERATIONS; ++iterations) {
		ret.resize(bufferSize);
		auto charsReturned = ::GetCurrentDirectoryW(bufferSize, (LPWSTR) ret.data());
		if (charsReturned < ret.length())
		{
			ret.resize(charsReturned);
			return ret;
		}
		else
		{
			bufferSize *= 2;
		}
	}
	return L"";
}

std::wstring GetModulePath(HMODULE hModule)
{
	static const auto INITIAL_BUFFER_SIZE = MAX_PATH;
	static const auto MAX_ITERATIONS = 7;
	std::wstring ret;
	auto bufferSize = INITIAL_BUFFER_SIZE;
	for (size_t iterations = 0; iterations < MAX_ITERATIONS; ++iterations)
	{
		ret.resize(bufferSize);
		size_t charsReturned = 0;
		charsReturned = GetModuleFileNameW(hModule, (LPWSTR) ret.data(), bufferSize);
		if (charsReturned < ret.length())
		{
			ret.resize(charsReturned);
			return ret;
		}
		else
		{
			bufferSize *= 2;
		}
	}
	return std::wstring();
}

std::wstring GetExeModulePath()
{
	std::wstring r = GetModulePath(NULL);
	r = r.substr(0, r.find_last_of(L"/\\") + 1);
	return r;
}


inline std::wstring GetSelfName()
{
	return moduleFileName.substr(moduleFileName.find_last_of(L"/\\") + 1);
}


enum Kernel32ExportsNames
{
	eGetStartupInfoA,
	eGetStartupInfoW,
	eGetModuleHandleA,
	eGetModuleHandleW,
	eGetProcAddress,
	eGetShortPathNameA,
	eFindFirstFileA,
	eFindNextFileA,
	eFindFirstFileW,
	eFindNextFileW,
	eFindFirstFileExA,
	eFindFirstFileExW,
	eLoadLibraryExA,
	eLoadLibraryExW,
	eLoadLibraryA,
	eLoadLibraryW,
	eFreeLibrary,
	eCreateEventA,
	eCreateEventW,
	eGetSystemInfo,
	eInterlockedCompareExchange,
	eSleep,
	eGetSystemTimeAsFileTime,
	eGetCurrentProcessId,
	eGetCommandLineA,
	eGetCommandLineW,
	eAcquireSRWLockExclusive,
	eCreateFileA,
	eCreateFileW,
	eGetFileAttributesA,
	eGetFileAttributesW,
	eGetFileAttributesExA,
	eGetFileAttributesExW,

	Kernel32ExportsNamesCount
};

enum Kernel32ExportsData
{
	IATPtr,
	ProcAddress,

	Kernel32ExportsDataCount
};

enum OLE32ExportsNames
{
	eCoCreateInstance,

	OLE32ExportsNamesCount
};

enum vccorlibExportsNames
{
	eGetCmdArguments,

	vccorlibExportsNamesCount
};

//size_t Kernel32Data[Kernel32ExportsNamesCount][Kernel32ExportsDataCount];
//size_t OLE32Data[OLE32ExportsNamesCount][Kernel32ExportsDataCount];
//size_t vccorlibData[vccorlibExportsNamesCount][Kernel32ExportsDataCount];



#include "injector/injector.hpp"
#include "utils/func.h"
#include "utils/log.h"
#include "ptle/types/types.h"

#include "ptle/EInstance.h"
#include "ptle/EIBeast.h"
#include "ptle/EIPlant.h"
#include "ptle/EIProjectile.h"
#include "ptle/EITreasureIdol.h"
#include "ptle/ESDInt.h"
#include "ptle/EScriptContext.h"

#include "ptle/containers/TreeMap/TreeMap.h"

#include "gizmod/event/BeastStateSwitchEvent.h"
#include "gizmod/event/BreakableBreakEvent.h"
#include "gizmod/event/CinematicPlayEvent.h"
#include "gizmod/event/CollectItemEvent.h"
#include "gizmod/event/EntitySpawnEvent.h"
#include "gizmod/event/LevelLoadedEvent.h"
#include "gizmod/event/LoadLevelEvent.h"
#include "gizmod/event/PlantRustleEvent.h"
#include "gizmod/event/ProjectileShootEvent.h"
#include "gizmod/event/ShamanPurchaseEvent.h"


// Monkey Temple crash fix.
GET_FUNC( 0x422C30, void, Script_SetBeastTarget_Original, EScriptContext* );
GET_METHOD( 0x6C2E70, void*, EScriptContext_PopVar, EScriptContext* );
void Script_SetBeastTarget_Safe( EScriptContext* context )
{
	EIBeast* beast = type_cast<EIBeast>( context->m_owningInstance, get_type_by_vtable(0x86C3D0) );
	if ( !beast ) {
		EScriptContext_PopVar( context );
		return;
	}

	Script_SetBeastTarget_Original( context );
}


GET_METHOD( 0x4159B0, void, EIBeast_PerformStateSwitch, EIBeast*, uint32_t, bool, bool );
GET_METHOD( 0x428A80, void, EIBushNinja_PerformStateSwitch, EIBeast*, uint32_t, bool, bool );
class MyBeast : public EIBeast
{
public:
	void PerformStateSwitch_custom( uint32_t stateID, bool param1, bool param2 )
	{
		EBeastStateMachine* sm = this->GetStateMachine();
		const char* name = sm->m_states[stateID].m_name;
		const char* nameEnd = name + strlen( name );

		BeastStateSwitchEvent event( this, stateID );
		g_pitfall.getEventListener()->invokeEvent( event );

		if ( !event.isCancelled() || strcmp(nameEnd - 4, "init") == 0 ) {
			if ( this->GetType() == get_type_by_vtable(0x86D1D8)->ptleType ) {
				EIBushNinja_PerformStateSwitch( this, stateID, param1, param2 );
			}
			else {
				EIBeast_PerformStateSwitch( this, stateID, param1, param2 );
			}
		}
	}
};

enum DamageSource
{
	HARRY_PUNCH,
	HARRY_SPINKICK,
	HARRY_CROUCHKICK,
	HARRY_RISING_STRIKE,
	HARRY_SMASH_STRIKE,
	HARRY_BREAKDANCE,
	HARRY_SHIELD,
	HARRY_PICKAXES,

	SLING_ROCK,
	SLING_SUPER,

	TNT_BLAST,
};
static const char* source( uint32_t s )
{
	if ( s & 0x1 ) return "Sling rock";
	if ( s & 0x4 ) return "Harry punch";
	if ( s & 0x8 ) return "Breakdance";
	if ( s & 0x20) return "Rising strike";
	if ( s & 0x80 ) return "Pickaxes";
	if ( s & 0x100 ) return "Supersling";
	if ( s & 0x200 ) return "Shield";
	if ( s & 0x400 ) return "TNT";
	if ( s & 0x1000 ) return "Coconut?";
	if ( s & 0x80000 ) return "Smash strike";
	return "idk";
}
GET_METHOD( 0x4EDF50, void, EIHavokBreakable_Break, EInstance*, uint32_t, Vector4f*, float );
static void _BreakableBreak( EInstance* self, uint32_t p4, Vector4f* p7, float p6 )
{
	BreakableBreakEvent event( self );
	g_pitfall.getEventListener()->invokeEvent( event );

	if ( !event.isCancelled() ) {
		EIHavokBreakable_Break( self, p4, p7, p6 );
	}
}
MAKE_THISCALL_WRAPPER( BreakableBreak, _BreakableBreak );

GET_METHOD( 0x506170, void, CollectItem, void*, uint32_t );
static void _CollectItem_custom( void* self, uint32_t itemHash )
{
	CollectItemEvent event( itemHash );
	if ( event.getItem() == 0 ) {   // For artifacts.
		return;
	}

	Gizmod::getInstance()->getEventListener()->invokeEvent( event );

	if ( !event.isCancelled() ) {
		CollectItem( self, event.getItemHash() );
	}
}
MAKE_THISCALL_WRAPPER( CollectItem_custom, _CollectItem_custom );

// VERY hacky :
// The original "AddCollectedIdols" function is not a member function. But at the injection point, ECX contains a pointer
// to the idol, which we can retrieve by pretending that it is a member function.
GET_FUNC( 0x5E34A0, void, AddCollectedIdols, uint32_t, int );
static void _AddCollectedIdols_custom( EITreasureIdol* self, uint32_t levelCRC, int amount )
{
	amount = self->GetAmount();
	CollectIdolEvent event( self, amount );
	g_pitfall.getEventListener()->invokeEvent( event );

	if ( !event.isCancelled() ) {
		AddCollectedIdols( levelCRC, event.getAmount() );
	}
}
MAKE_THISCALL_WRAPPER( AddCollectedIdols_custom, _AddCollectedIdols_custom );

// Cutscene start.
GET_METHOD( 0x4308B0, bool, EICinematic_CheckCannotPlay, EICinematic*, bool );
static bool _CinematicCannotPlay( EICinematic* self, bool param )
{
	// Determine whether the cutscene will play at all, using the original function.
	bool notPlay = EICinematic_CheckCannotPlay( self, param );
	if ( notPlay ) return true;

	// If it is going to, forward the event to the manager.
	CinematicPlayEvent event( self );
	g_pitfall.getEventListener()->invokeEvent( event );

	return event.isCancelled();
}
MAKE_THISCALL_WRAPPER( CinematicCannotPlay, _CinematicCannotPlay );


// Plant rustle.
static void _PlantRustleBig( EIPlant* plant )
{
	// Ignore if already rustling.
	if ( plant->m_state1 != 0 || plant->m_state0 != 0 ) return;

	PlantRustleEvent event( plant );
	g_pitfall.getEventListener()->invokeEvent( event );

	if ( !event.isCancelled() ) {
		plant->m_state1 = 2;
	}
}
MAKE_THISCALL_WRAPPER( PlantRustleBig, _PlantRustleBig );

static void _PlantRustleSmall( EIPlant* plant )
{
	// Ignore if already rustling.
	if ( plant->m_state1 != 0 || plant->m_state0 != 0 ) return;

	PlantRustleEvent event( plant );
	g_pitfall.getEventListener()->invokeEvent( event );

	if ( !event.isCancelled() ) {
		plant->m_state1 = 1;
	}
}
static void __declspec(naked) PlantRustleSmall()
{
	__asm push edx     // "this" pointer is in EDX at the point of injection.
	__asm call dword ptr _PlantRustleSmall
	__asm pop edx
	__asm ret
}


// Projectile shoot.
GET_METHOD( 0x559640, void, EIProjectile_Shoot, EIProjectile*, const Vector3f*, const Vector3f*, EInstance*, float );
static void __stdcall _ProjectileShoot( EIProjectile* self, Vector3f* position, Vector3f* direction, EInstance* source, float accuracy )
{
	ProjectileShootEvent event( self, source, *position, *direction );
	g_pitfall.getEventListener()->invokeEvent( event );

	if ( !event.isCancelled() ) {
		EIProjectile_Shoot( self, &event.getPosition(), &event.getDirection(), source, accuracy );
	}
	else {
		self->~EIProjectile();
	}
}
MAKE_THISCALL_WRAPPER( ProjectileShoot, _ProjectileShoot );


static void EntitySpawn( class ERLevel* level, EInstance* inst )
{
	int step = *((int*) 0x91707C);
	//log_printf( "Spawned! %s\n", inst->GetTypeName() );

	EntitySpawnEvent event( inst );
	g_pitfall.getEventListener()->invokeEvent<EntitySpawnEvent>( event );
}

static __declspec(naked) void _EntitySpawn()
{
	__asm push dword ptr [ebp+0x8]
	__asm push ecx
	__asm call EntitySpawn
	__asm mov esp, ebp
	__asm pop ebp
	__asm ret 0x4
}

GET_METHOD( 0x5229B0, void, EMusicManager_PlayMusic, void*, uint32_t, bool );
static void LevelLoaded()
{
	uint32_t globalStruct = 0x917028;
	uint32_t obj1 =     *(uint32_t*)(globalStruct + 0x4);
	uint32_t obj2 =     *(uint32_t*)(obj1         + 0x4);
	uint32_t musicCRC = *(uint32_t*)(obj2         + 0x34);

	LevelLoadedEvent event;
	event.setMusicOverride( musicCRC );
	g_pitfall.getEventListener()->invokeEvent( event );

	void* musicManager = (void*) 0x910850;
	if ( event.getMusicCRC() != 0 ) {
		EMusicManager_PlayMusic( musicManager, event.getMusicCRC(), true );
	}
}

GET_METHOD( 0x5EBA90, void, LoadLevel, void*, uint32_t, bool );
static uint32_t* get_prev_level_address()
{
	TreeMap* map = ((TreeMap*) 0x91FE2C);      // This tree map contains a node which points to the previous area value...
	const uint32_t targetNodeID = 0x174CD628;  // ... and this is the ID of said node.

	// Look for it.
	// TODO : Binary search might be a good idea for performance.
	TreeMapNode* node = map->m_iterateFirst;
	while ( node && node->m_hash != targetNodeID ) {
		node = node->m_iterateNext;
	}

	ESDInt* scriptInt = reinterpret_cast<ESDInt*>(node->m_ptr);
	return reinterpret_cast<uint32_t*>(&scriptInt->m_value);
}
static void _LoadLevel_custom( void* self, uint32_t levelCRC, bool param )
{
	uint32_t* prevPtr = get_prev_level_address();
	LoadLevelEvent event( levelCRC, *prevPtr );
	Gizmod::getInstance()->getEventListener()->invokeEvent( event );

	levelCRC = event.getLevelCRC();
	*prevPtr = event.getPrevCRC();

	LoadLevel( self, levelCRC, param );
}
MAKE_THISCALL_WRAPPER( LoadLevel_custom, _LoadLevel_custom )

static ShamanShop::PriceSlot id_to_shaman( int id )
{
	switch ( id )
	{
	case 0x89: return ShamanShop::HEALTH1;
	case 0x8A: return ShamanShop::CANTEEN1;
	case 0x8B: return ShamanShop::SMASH_STRIKE;
	case 0x8C: return ShamanShop::SUPER_SLING;
	case 0x8D: return ShamanShop::BREAKDANCE;
	case 0x8E: return ShamanShop::JUNGLE_NOTES;
	case 0x8F: return ShamanShop::NATIVE_NOTES;
	case 0x90: return ShamanShop::CAVERN_NOTES;
	case 0x91: return ShamanShop::MOUNTAIN_NOTES;
	case 0x9C: return ShamanShop::MYSTERY_ITEM;
	default: return ShamanShop::UNKNOWN;
	}
}
static bool OnShamanPurchase( int id )
{
	ShamanPurchaseEvent event( id_to_shaman(id) );
	g_pitfall.getEventListener()->invokeEvent( event );

	return event.isCancelled();
}

class EPauseMain;
GET_METHOD( 0x52C590, void, EPauseMain_Message, EPauseMain*, int, void*, int );
static void _EPauseMain_Message_custom( EPauseMain* self, int messageID, void* param1, int param2 )
{
	switch ( messageID )
	{
	// Shaman purchase.
	case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F: case 0x90: case 0x91: case 0x9C:
		if ( OnShamanPurchase( messageID ) ) {
			return;
		}
		break;
	}

	EPauseMain_Message( self, messageID, param1, param2 );
}
MAKE_THISCALL_WRAPPER( EPauseMain_Message_custom, _EPauseMain_Message_custom );

static bool ReturnYes()
{
	return true;
}

static void LateInit()
{
	// Beast state change.
	const ETypeInfo* beast = get_type_by_vtable( 0x86C3D0 )->ptleType;
	const ETypeInfo* flockBeast = get_type_by_vtable( 0x876D28 )->ptleType;

	for ( auto p : get_all_types() ) {
	const ETypeInfo* baseType = p.second.ptleType->m_parent;
		if ( baseType == beast || baseType == flockBeast ) {
			void** vtable = (void**) p.first;
			injector::WriteMemory( &vtable[0xB9], &MyBeast::PerformStateSwitch_custom );
		}
	}

	injector::MakeJMP( 0x620053, 0x62006A );
}

void InjectCode()
{
	// Inconveniently spammed prints remaining in the game's code.
	injector::MakeRangedNOP( 0x6824CF, 0x6824DC );    // Remove "Its in the Box!!" message.
	injector::MakeRangedNOP( 0x60D1B6, 0x60D1C3 );    // "ACTIVATE!!!!"

	// Bonus : Replace this script function with one that null-checks first. This protects against Monkey Temple crash.
	injector::WriteMemory( 0x8F0A0C, &Script_SetBeastTarget_Safe );

	// Protection against explorer softlock.
	injector::WriteMemory( 0x87659C, ReturnYes );

	// Skip splash screens and trailer.
	if ( g_skipSplashScreens ) {
		injector::WriteMemory<uint8_t>( 0x512BC5, 0x22 );

		injector::WriteMemory<uint8_t>( 0x489929, 3 );
	}

	// Late init.
	injector::MakeRangedNOP( 0x60D439, 0x60D441 );
	injector::MakeCALL( 0x60D439, LateInit );
	injector::MakeRangedNOP( 0x60E977, 0x60E97F );


	// Breakables.
	injector::MakeCALL( 0x4EDD91, BreakableBreak );

	// Collect items & idols.
	injector::MakeCALL( 0x4E9E51, CollectItem_custom );        // Intercept item unlock ("HarryAddInventoryItem" script function).
	injector::MakeCALL( 0x598036, CollectItem_custom );        // Intercept item unlock (picking up an EITreasureIdol).
	injector::MakeCALL( 0x598004, AddCollectedIdols_custom );  // Intercept idol grab (picking up an EITreasureIdol).
	injector::MakeRangedNOP( 0x597FE8, 0x597FFF );

	// Cinematic playing.
	injector::MakeCALL( 0x430A20, CinematicCannotPlay );

	// Plants rustling.
	injector::MakeNOP( 0x5444C6, 10 );
	injector::MakeCALL( 0x5444C6, PlantRustleBig );
	injector::MakeNOP( 0x545222, 10 );
	injector::MakeCALL( 0x545222, PlantRustleSmall );
	injector::MakeNOP( 0x545507, 10 );
	injector::MakeCALL( 0x545507, PlantRustleSmall );

	// Projectile shooting.
	static const uint32_t callLocations[] = { 0x494253, 0x501F12, 0x556723, 0x559B23, 0x559EA5, 0x559FFA, 0x55E0A4, 0x583711, 0x583B77, 0x595867, 0x595960 };
	for ( int i = 0; i < _countof(callLocations); i++ ) {
		injector::MakeCALL( callLocations[i], ProjectileShoot );
	}


	// Not working.
	//injector::MakeJMP( 0x626747, _EntitySpawn );

	// Load level.
	injector::MakeCALL( 0x5ECC70, LoadLevel_custom );

	// Level finished loading.
	injector::MakeNOP( 0x5EC196, 8 );
	injector::MakeCALL( 0x5EC196, LevelLoaded );
	injector::MakeRangedNOP( 0x5096BC, 0x5096ED );             // Defer music playing.

	// EPauseMain_Message() in vtable.
	injector::WriteMemory( 0x88E484, &EPauseMain_Message_custom );
}



void LoadOriginalLibrary()
{
	auto szSelfName = GetSelfName();
	auto szSystemPath = SHGetKnownFolderPath(FOLDERID_System, 0, nullptr) + L'\\' + szSelfName;
	auto szLocalPath = moduleFileName.substr( 0, moduleFileName.find_last_of(L"/\\") + 1 );

	if ( iequals(szSelfName, L"binkw32.dll") ) {
		szLocalPath += L"binkw32_o.dll";
		HMODULE ogLib = LoadLibraryW(szLocalPath.c_str());
		if ( ogLib != 0 ) {
			binkw32.LoadOriginalLibrary(ogLib, false);
		}
	}
	else {
		MessageBox(0, TEXT("This library isn't supported."), TEXT("ASI Loader"), MB_ICONERROR);
		ExitProcess(0);
	}
}


static bool matchesTargetExtension( LPWSTR filename )
{
	auto length = wcslen( filename );
	return filename[length - 4] == '.' &&
		(filename[length - 3] == 'a' || filename[length - 3] == 'A') &&
		(filename[length - 2] == 's' || filename[length - 2] == 'S') &&
		(filename[length - 1] == 'i' || filename[length - 1] == 'I');
}

static std::string getFileNameWithoutExt( const std::wstring& path )
{
	size_t dirsep = path.find_last_of( L'\\' );
	size_t extdot = path.find_last_of( L'.' );
	std::wstring name = path.substr( dirsep + 1, extdot - dirsep - 1 );

	return std::string( name.begin(), name.end() );
}

void FindFiles()
{
	WIN32_FIND_DATAW findData;
	WIN32_FIND_DATAW* fd = &findData;

	std::wstring gameDir = GetCurrentDirectoryW();
	std::wstring modsDir = gameDir + L"\\mods";

	HANDLE asiFile = FindFirstFileW(L".\\mods\\*.asi", fd);
	if ( asiFile == INVALID_HANDLE_VALUE ) {
		return;
	}

	// List all ASIs present in the mods directory.
	std::vector<std::wstring> modulesToLoad;
	do {
		if ((fd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) continue;

		if ( matchesTargetExtension(fd->cFileName) ) {
			std::wstring path = modsDir + L'\\' + fd->cFileName;

			if (GetModuleHandleW(path.c_str()) != NULL) continue;

			modulesToLoad.push_back( path );
		}
	}
	while (FindNextFileW(asiFile, fd));

	FindClose(asiFile);

	// Load all.
	int success = 0;
	log_printf( "Loading %d mods...\n\n", modulesToLoad.size() );
	for ( const std::wstring& path : modulesToLoad ) {
		HMODULE h = LoadLibraryW( path.c_str() );
		SetCurrentDirectoryW( gameDir.c_str() );   // In case asi changed it.

		// Invoke plugin enable here.
		Gizmod** modLoaderPtr = (Gizmod**) GetProcAddress( h, "gizmodInstance" );
		GizmodPlugin* plugin = (GizmodPlugin*) GetProcAddress( h, "pluginInstance" );
		if ( !modLoaderPtr || !plugin ) {
			log_printf( "ERROR : Mod file \"%s\" has an invalid signature.\n", path.c_str() );
			continue;
		}

		int ver = Gizmod::checkVersion( plugin );
		if ( ver == 2 ) {
			log_printf( "ERROR : Mod \"%s\" was made for a later version of Gizmod. It will not be loaded.\n", plugin->getDisplayName() );
			continue;
		}
		if ( ver == -2 ) {
			log_printf( "ERROR : Mod \"%s\" was made for an earlier version of Gizmod. It will not be loaded.\n", plugin->getDisplayName() );
			continue;
		}
		if ( ver == -1 ) {
			log_printf( "WARN : Mod \"%s\" was made for an earlier patch of Gizmod. Some features might not work properly.\n", plugin->getDisplayName() );
		}

		// Warn if names don't match (might become a requirement).
		if ( getFileNameWithoutExt(path) != plugin->getSystemName() ) {
			log_printf( "WARN : Mod \"%s\"'s system name and file name do not match. This might become a requirement in the future.\n", plugin->getDisplayName() );
		}

		log_printf( "[%s] Starting...\n", plugin->getDisplayName() );
		*modLoaderPtr = &g_pitfall;
		plugin->onEnable();
		log_printf( "[%s] Mod started.\n", plugin->getDisplayName() );

		success++;
	}

	log_printf( "\n%d mods loaded successfully.\n\n", success );
}

void LoadPlugins()
{
	auto oldDir = GetCurrentDirectoryW();

	auto szSelfPath = moduleFileName.substr(0, moduleFileName.find_last_of(L"/\\") + 1);
	SetCurrentDirectoryW(szSelfPath.c_str());

	FindFiles();

	SetCurrentDirectoryW(oldDir.c_str());
}


#define value_orA(path1, path2) (path1.empty() ? path2 : path1.string().c_str())
#define value_orW(path1, path2) (path1.empty() ? path2 : path1.wstring().c_str())



LONG WINAPI CustomUnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo)
{
	// step 1: write minidump
	wchar_t     modulename[MAX_PATH];
	wchar_t     filename[MAX_PATH];
	wchar_t     timestamp[128];
	__time64_t  time;
	struct tm   ltime;
	HANDLE      hFile;
	HWND        hWnd;

	wchar_t* modulenameptr = nullptr;
	if (GetModuleFileNameW(GetModuleHandle(NULL), modulename, _countof(modulename)) != 0)
	{
		modulenameptr = wcsrchr(modulename, '\\');
		*modulenameptr = L'\0';
		modulenameptr += 1;
	}
	else
	{
		wcscpy_s(modulename, L"err.err");
	}

	_time64(&time);
	_localtime64_s(&ltime, &time);
	wcsftime(timestamp, _countof(timestamp), L"%Y%m%d%H%M%S", &ltime);
	swprintf_s(filename, L"%s\\%s\\%s.%s.dmp", modulename, L"CrashDumps", modulenameptr, timestamp);

	hFile = CreateFileW(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ex;
		memset(&ex, 0, sizeof(ex));
		ex.ThreadId = GetCurrentThreadId();
		ex.ExceptionPointers = ExceptionInfo;
		ex.ClientPointers = TRUE;

		if (FAILED(MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithDataSegs, &ex, NULL, NULL)))
		{
		}

		CloseHandle(hFile);
	}

	// step 2: write log
	// Logs exception into buffer and writes to file
	swprintf_s(filename, L"%s\\%s\\%s.%s.log", modulename, L"CrashDumps", modulenameptr, timestamp);
	hFile = CreateFileW(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		auto Log = [ExceptionInfo, hFile](char* buffer, size_t size, bool reg, bool stack, bool trace)
		{
			if (LogException(buffer, size, (LPEXCEPTION_POINTERS)ExceptionInfo, reg, stack, trace))
			{
				DWORD NumberOfBytesWritten = 0;
				WriteFile(hFile, buffer, strlen(buffer), &NumberOfBytesWritten, NULL);
			}
		};

		// Try to make a very descriptive exception, for that we need to malloc a huge buffer...
		if (auto buffer = (char*)malloc(max_logsize_ever))
		{
			Log(buffer, max_logsize_ever, true, true, true);
			free(buffer);
		}
		else
		{
			// Use a static buffer, no need for any allocation
			static const auto size = max_logsize_basic + max_logsize_regs + max_logsize_stackdump;
			static char static_buf[size];
			static_assert(size <= max_static_buffer, "Static buffer is too big");

			Log(buffer = static_buf, sizeof(static_buf), true, true, false);
		}

		CloseHandle(hFile);
	}

	// step 3: exit the application
	ShowCursor(TRUE);
	hWnd = FindWindowW(0, L"");
	SetForegroundWindow(hWnd);

	return EXCEPTION_CONTINUE_SEARCH;
}


#include "modloader/modloader.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH) {
		wchar_t filename[8192];

		gizmodInstance = &g_pitfall;

		register_types();

		load_config();

		log_printf( "Gizmod %s\n", g_pitfall.getVersionString().c_str() );

		hm = hModule;
		GetModuleFileNameW( hm, filename, sizeof(filename) );
		moduleFileName = filename;

		// Load binkw32.
		LoadOriginalLibrary();

		// Load mods.
		if ( g_enabled ) {
			InjectCode();

			LoadPlugins();
		}
	}
	else if (reason == DLL_PROCESS_DETACH) {
		/*for (size_t i = 0; i < OLE32ExportsNamesCount; i++) {
			if (OLE32Data[i][IATPtr] && OLE32Data[i][ProcAddress]) {
				auto ptr = (size_t*)OLE32Data[i][IATPtr];
				DWORD dwProtect[2];
				VirtualProtect(ptr, sizeof(size_t), PAGE_EXECUTE_READWRITE, &dwProtect[0]);
				*ptr = OLE32Data[i][ProcAddress];
				VirtualProtect(ptr, sizeof(size_t), dwProtect[0], &dwProtect[1]);
			}
		}*/
	}
	return TRUE;
}
