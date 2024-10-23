#pragma once

#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <stdexcept>
#include <cassert>

class VirtualProtectGuard
{
public:
	VirtualProtectGuard(void* base, size_t len, std::uint32_t flags)
	{
		_base = base;
		_length = len;
		if (!VirtualProtect(base, len, flags, (PDWORD)&_old))
			throw std::runtime_error(_("Failed to protect region."));
	}

	~VirtualProtectGuard()
	{
		VirtualProtect(_base, _length, _old, (PDWORD)&_old);
	}

private:
	void* _base;
	size_t _length;
	std::uint32_t _old;
};