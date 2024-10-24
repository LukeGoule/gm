#include "compilestring.h"

#include "../hooks.h"
#include "../../helpers/utils.h"
#include "../../hacks/lualog.h"

#include "../../gmod/luaShared.h"

namespace Hooks 
{
	//static void onCompileString(lua::ILuaInterface* pThis, void* _edx, /*Bootil::Buffer &*/ void* dumper, const std::string* stringToCompile)
	//{
	//	LuaLog::Get().onCompileString(pThis, dumper, stringToCompile);
	//}

	bool __fastcall hkCompileString(lua::ILuaInterface* pThis, void* _edx, /*Bootil::Buffer &*/ void* dumper, const std::string* stringToCompile)
	{
		static decltype(&hkCompileString) oCompileString = GetLuaHookmanForInterface(pThis).get_original<decltype(&hkCompileString)>(Hooks::Indexes::ILuaInterface::CompileString);
		//onCompileString(pThis, _edx, dumper, stringToCompile);
		return oCompileString(pThis, _edx, dumper, stringToCompile);
	}

}