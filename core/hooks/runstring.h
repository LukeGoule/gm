#pragma once

#ifndef __RUNSTRING_H__
#define __RUNSTRING_H__

namespace lua 
{
	class ILuaInterface;
}

namespace Hooks 
{
	bool __fastcall hkRunString(lua::ILuaInterface* pThis, void* _edx, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors);
}

#endif