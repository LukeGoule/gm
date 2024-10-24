#pragma once

#ifndef __RUNSTRINGEX_H__
#define __RUNSTRINGEX_H__

namespace lua 
{
	class ILuaInterface;
}

namespace Hooks 
{
	bool __fastcall hkRunStringEx(
		lua::ILuaInterface* pThis, 
		void* _edx, 
		const char* szFileName, 
		const char* szFilePath, 
		const char* szLuaString, 
		bool bRun, 
		bool bShowErrors, 
		bool bDontPushErrors, 
		bool bNoReturns
	);
}

#endif