#pragma once

#ifndef __COMPILESTRING_H__
#define __COMPILESTRING_H__

#include <string>

namespace lua 
{
	class ILuaInterface;
}

namespace Hooks 
{
	bool __fastcall hkCompileString(
		lua::ILuaInterface* pThis, 
		void* _edx, 
		/*Bootil::Buffer &*/ void* dumper, 
		const std::string* stringToCompile
	);
}

#endif