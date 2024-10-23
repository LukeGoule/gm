#pragma once

#define NOMINMAX
#include <Windows.h>

#include <cstdint>
#include <stdexcept>
#include <cassert>

#include "../helpers/obfs.h"
#include "VirtualProtectGuard.h"

class VirtualFunctionHook
{
public:
	VirtualFunctionHook();
	VirtualFunctionHook(void* base);
	~VirtualFunctionHook();

	bool setup(void* class_base = nullptr);

	template<typename T>
	void hook_index(int index, T fun)
	{
		assert(index >= 0 && index <= (int)vftbl_len);
		new_vftbl[index + 1] = reinterpret_cast<std::uintptr_t>(fun);
	}
	void unhook_index(int index)
	{
		new_vftbl[index] = old_vftbl[index];
	}
	void unhook_all()
	{
		try {
			if (old_vftbl != nullptr) {
				auto guard = VirtualProtectGuard{ class_base, sizeof(std::uintptr_t), PAGE_READWRITE };
				*(std::uintptr_t**)class_base = old_vftbl;
				old_vftbl = nullptr;
			}
		}
		catch (...) {
		}

		class_base = nullptr;
	}

	template<typename T>
	T get_original(int index)
	{
		return (T)old_vftbl[index];
	}

	void* class_base;
private:
	static inline std::size_t estimate_vftbl_length(std::uintptr_t* vftbl_start);
	std::size_t     vftbl_len;
	std::uintptr_t* new_vftbl;
	std::uintptr_t* old_vftbl;
};