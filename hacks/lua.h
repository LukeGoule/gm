#pragma once

#ifndef __LUA_HACK_H__
#define __LUA_HACK_H__

#include <iostream>
#include <deque>
#include <vector>
#include <string>

#include "../helpers/singleton.h"

namespace lua
{
	class ILuaInterface;
}

class LuaLog : public Singleton<LuaLog>
{
public:

	struct LogItem
	{
		lua::ILuaInterface* m_pInterface;
		std::string m_szFileName;
		std::string m_szFilePath;
		std::string m_szLuaString;
		bool m_bRun;
		bool m_bShowErrors;
	};

private:
	
	std::deque<LuaLog::LogItem*> m_Stack;
	size_t m_iMaxSize = 500;

public:

	void onRunString(lua::ILuaInterface* pLuaInterface, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors);
	void onRunStringEx(lua::ILuaInterface* pLuaInterface, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors, bool bDontPushErrors, bool bNoReturns);
	void onCompileString(lua::ILuaInterface* pLuaInterface, void* dumper, const std::string* stringToCompile);

	std::vector<const char*> listFileNames();
	LuaLog::LogItem* getItem(size_t iIndex);
};

#endif