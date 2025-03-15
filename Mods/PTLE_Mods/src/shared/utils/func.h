#pragma once


// Create a function pointer to a known non-member function.
#define GET_FUNC( addr, rettype, name, ... ) \
	static rettype (*name)(__VA_ARGS__) = reinterpret_cast<rettype(*)(__VA_ARGS__)>(addr)


// Create a function pointer to a known method (member function).
#define GET_METHOD( addr, rettype, name, ... ) \
	static rettype (__thiscall *name)(__VA_ARGS__) = reinterpret_cast<rettype(__thiscall *)(__VA_ARGS__)>(addr)


// Make a custom function compatible with the __thiscall convention.
// ---
// The "this" parameter is passed in ECX, which we forward to the custom function as
// a regular stack parameter.
#define MAKE_THISCALL_WRAPPER( funcName, funcPtr ) \
	static __declspec(naked) void funcName() { \
		__asm pop edi \
		__asm push ecx \
		__asm push edi \
		__asm jmp dword ptr funcPtr \
	}
