#include "compilestring.h"

#include "../hooks.h"
#include "../../helpers/utils.h"
#include "../../hacks/lua.h"

namespace Hooks 
{
	static void onCompileString(lua::ILuaInterface* pThis, void* _edx, /*Bootil::Buffer &*/ void* dumper, const std::string* stringToCompile)
	{
		LuaLog::Get().onCompileString(pThis, dumper, stringToCompile);
	}

	bool __fastcall hkCompileString(lua::ILuaInterface* pThis, void* _edx, /*Bootil::Buffer &*/ void* dumper, const std::string* stringToCompile)
	{
		static decltype(&hkCompileString) oCompileString = luainterface_hook.get_original<decltype(&hkCompileString)>(Hooks::Indexes::ILuaInterface::CompileString);
		onCompileString(pThis, _edx, dumper, stringToCompile);
		return oCompileString(pThis, _edx, dumper, stringToCompile);
	}

}