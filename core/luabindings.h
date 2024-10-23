#pragma once

#include "../core/globals.h"
#include "../gmod/luaShared.h"
#include "../lua_source/lua.hpp"

#include <vector>

#define CL_LUA g_pLuaShared->GetLuaInterface(LUAINTERFACE_CLIENT)

class LuaBinding_Func {
public:
	char* m_pszName;
	CFunc m_Function;

	LuaBinding_Func(const char* pszName, CFunc Function);
};

class LuaBindings {
public:
	LuaBindings();
	~LuaBindings();

	std::vector<LuaBinding_Func*> LuaBoundFunctions;

	void NewFunction(const char* pszName, CFunc Function);
	void SetupFunctions();
	void CreateFunctions(lua::ILuaInterface* LUA);

	void ExecString(lua::ILuaInterface* LUA, std::string str); // wrapper for LUA->RunString
};

extern LuaBindings* g_pLuaBindings;