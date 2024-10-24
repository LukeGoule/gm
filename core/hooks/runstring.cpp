#include "runstring.h"

#include "../hooks.h"
#include "../../helpers/utils.h"
#include "../../hacks/lua.h"

namespace Hooks 
{
	static void onRunString(lua::ILuaInterface* pThis, void* _edx, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors)
	{
		LuaLog::Get().onRunString(pThis, szFileName, szFilePath, szLuaString, bRun, bShowErrors);
	}

	bool __fastcall hkRunString(lua::ILuaInterface* pThis, void* _edx, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors) 
	{
		static decltype(&hkRunString) oRunString = luainterface_hook.get_original<decltype(&hkRunString)>(Hooks::Indexes::ILuaInterface::RunString);
		onRunString(pThis, _edx, szFileName, szFilePath, szLuaString, bRun, bShowErrors);
		return oRunString(pThis, _edx, szFileName, szFilePath, szLuaString, bRun, bShowErrors);
	}

}