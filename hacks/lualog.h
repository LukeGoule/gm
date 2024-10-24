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

class RunstringLuaScript
{
public:

	lua::ILuaInterface* m_pLuaInterface;
	std::string m_szFileName;
	std::string m_szFilePath;
	std::string m_szLuaString;
	std::string m_label;

	bool m_bRun;
	bool m_bShowErrors;
};


class LuaLog : public Singleton<LuaLog>
{
private:
	
	//std::deque<RunstringLuaScript*> m_Stack;
	std::vector<RunstringLuaScript*> m_scripts;
	size_t m_iMaxSize = 500;

public:

	void onRunString(lua::ILuaInterface* pLuaInterface, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors);
	void onRunStringEx(lua::ILuaInterface* pLuaInterface, const char* szFileName, const char* szFilePath, const char* szLuaString, bool bRun, bool bShowErrors, bool bDontPushErrors, bool bNoReturns);

	std::vector<const char*> listFileNames();
	RunstringLuaScript* getItem(size_t iIndex);
	std::string getOutputFolder();
	void saveFile(std::string fullPath, std::string fileData);
};

#endif