#pragma once

#include <cstdint>
#include <array>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#undef interface

#include <assert.h>

#include "../helpers/obfs.h"

template<typename out, class type>
inline out method(size_t index, type* self)
{
	return reinterpret_cast<out>((*reinterpret_cast<void***>(self))[index]);
}

template< typename Function > Function GetVFunc(PVOID Base, DWORD Index) // TODO: FIX GOWNOCODE
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];
	return (Function)(dwAddress);
}

inline void* GetFunction(void* Instance, int Index)
{
	DWORD VirtualFunction = (*(DWORD*)Instance) + sizeof(DWORD) * Index;
	return (void*)*((DWORD*)VirtualFunction);
}

struct interface_iterator_t
{
	void*(*create_interface)();
	char* name;
	interface_iterator_t* next;
};

inline uintptr_t follow_jmp(uintptr_t ptr)
{
	return ptr + 5 + *reinterpret_cast<int*>(ptr + 1);
}

inline auto get_interface_list(uintptr_t ptr)
{
	return **reinterpret_cast<interface_iterator_t***>(ptr + 0x6);
}

// Fasterified
inline size_t extract_version(const char* name)
{
	size_t version = 0;

	for (; *name; ++name)
	{
		if (*name <= '9')
			version = version * 10 + (*name - '0');
	}

	return version;
}

template<class type>
type* interface_get(HMODULE dll, const char* name)
{
	assert(dll);

	auto ptr = reinterpret_cast<uintptr_t>(GetProcAddress(dll, _("CreateInterface"))) + 0x4;
	assert(ptr - 0x4);

	ptr = follow_jmp(ptr);
	type*  interface = nullptr;
	size_t version = 0;

	for (auto iter = get_interface_list(ptr); iter; iter = iter->next)
	{
		if (std::strstr(iter->name, name))
		{
			if (auto current_version = extract_version(iter->name); current_version > version)
			{
				version = current_version;
				interface = reinterpret_cast<type*>(iter->create_interface());
			}
		}
	}

	return interface;
}
