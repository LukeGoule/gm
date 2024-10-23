#pragma once

#include "Interface.h"

class IEntityList
{
public:
	inline void* GetClientEntity(int idx)
	{
		using get_entity_t = void*(__thiscall*)(void*, int);
		return method<get_entity_t>(3, this)(this, idx);
	}

	inline void* GetClientEntityFromHandle(int idx)
	{
		using get_entity_from_handle_t = void*(__thiscall*)(void*, int);
		return method<get_entity_from_handle_t>(4, this)(this, idx);
	}

	inline size_t GetHighestEntityIndex()
	{
		using get_highest_entity_index_t = size_t(__thiscall*)(void*);
		return method<get_highest_entity_index_t>(6, this)(this);
	}
};