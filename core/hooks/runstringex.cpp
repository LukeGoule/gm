#include "runstringex.h"

#include "../hooks.h"
#include "../../helpers/utils.h"
#include "../../hacks/lualog.h"

#include "../../gmod/luaShared.h"

namespace Hooks 
{
	static void onRunStringEx(lua::ILuaInterface* pThis, void* _edx, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors, bool bDontPushErrors, bool bNoReturns)
	{
		LuaLog::Get().onRunStringEx(pThis, szFileName, szFilePath, szLuaString, bRun, bShowErrors, bDontPushErrors, bNoReturns);
	}

	bool __fastcall hkRunStringEx(lua::ILuaInterface* pThis, void* _edx, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors, bool bDontPushErrors, bool bNoReturns)
	{
		static decltype(&hkRunStringEx) oRunStringEx = GetLuaHookmanForInterface(pThis).get_original<decltype(&hkRunStringEx)>(Hooks::Indexes::ILuaInterface::RunStringEx);
		onRunStringEx(pThis, _edx, szFileName, szFilePath, szLuaString, bRun, bShowErrors, bDontPushErrors, bNoReturns);
		return oRunStringEx(pThis, _edx, szFileName, szFilePath, szLuaString, bRun, bShowErrors, bDontPushErrors, bNoReturns);
	}
}