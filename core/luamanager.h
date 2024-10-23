#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "../helpers/obfs.h"

/* @TODO */
#define LUAS_FOLDER _("C:\\loki_luas")

class LuaScript {
public:
	std::string m_ScriptName;
	std::string m_ScriptCode;

	LuaScript(std::string Name, std::string Code) {
		this->m_ScriptName = Name;
		this->m_ScriptCode = Code;
	}
};

class ScriptManager {
public:
	std::vector<LuaScript*> m_vecScriptList;

	ScriptManager();

	void HandleDefaultScript(std::string name, std::string code);

	std::vector<std::string> GetScriptNameList();
	std::vector<std::string> GetScriptCodeList();

	const char* GetScriptCodeByName(std::string name);
	LuaScript* GetScriptByName(std::string name);

	// if the name exists, overwrite it, if it doesn't then create a new object.
	LuaScript* SaveScript(std::string name, std::string code);
};

extern ScriptManager* g_pLuaScripts;