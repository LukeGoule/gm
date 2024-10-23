#include "luamanager.h"

#include "../helpers/utils.h"

#include <Windows.h>
#include <direct.h> // _mkdir
#include <fstream>
#include <filesystem>
#include <istream>
#include <sstream>

#define SCRIPT(s) std::string(std::string(LUAS_FOLDER) + std::string(_("\\")) + std::string(s) + std::string(_(".loki.lua"))) // mega bruh

bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

bool scriptFileExists(std::string scriptname) {
	GetFileAttributesA(SCRIPT(scriptname).c_str()); // from winbase.h
	
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributesA(SCRIPT(scriptname).c_str()) && GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		return false;
	}

	return true;
}

void read_directory(const std::string& name, std::vector<std::string>* v)
{
	std::string pattern(name);
	pattern.append("\\*");
	WIN32_FIND_DATAA data;
	HANDLE hFind;
	if ((hFind = FindFirstFileA(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {
			v->push_back(data.cFileName);
		} while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
	}
}

std::vector<std::string> explode(std::string const& s, char delim)
{
	std::vector<std::string> result;
	std::istringstream iss(s);

	for (std::string token; std::getline(iss, token, delim); )
	{
		result.push_back(std::move(token));
	}

	return result;
}

void ScriptManager::HandleDefaultScript(std::string name, std::string code) {
	this->m_vecScriptList.push_back(new LuaScript(name, code));

	std::ofstream output(SCRIPT(name));
	output << code;
	output.close();
}

ScriptManager::ScriptManager() {
	Utils::ConsolePrint(_("[Lua Script Loader] Checking for lua folder..\n"));

	if (!dirExists(LUAS_FOLDER)) {
		Utils::ConsolePrint(_("[Lua Script Loader] Folder doesn't exist, creating.\n"));
		_mkdir(LUAS_FOLDER);
	}
	else {
		Utils::ConsolePrint(_("[Lua Script Loader] Found folder.\n"));
	}

	if (!scriptFileExists(_("rainbow_physgun"))) {
		Utils::ConsolePrint(_("[Lua Script Loader] rainbow_physgun doesn't exist, loading default & writing.\n"));

		this->HandleDefaultScript(_("rainbow_physgun"),
			_(
			"function SetPhysgunRainbow( )\n"
			"	if (LocalPlayer():Alive()) then\n"
			"		if (LocalPlayer():GetActiveWeapon():GetClass() == \"weapon_physgun\") then\n"
			"			local time = CurTime()\n"
			"			local r = math.abs(math.sin(time * 0.5) * 255);\n"
			"			local g = math.abs(math.sin(time * 0.5 + 2) * 255);\n"
			"			local b = math.abs(math.sin(time * 0.5 + 4) * 255);\n"
			"			LocalPlayer():SetWeaponColor(Vector(r,g,b))\n"
			"		end\n"
			"	end\n"
			"end\n"
			"hook.Add(\"RenderScene\", \"rainbow_phys\", SetPhysgunRainbow)\n")
		);
	}

	if (!scriptFileExists(_("custom_crosshair"))) {
		Utils::ConsolePrint(_("[Lua Script Loader] custom_crosshair doesn't exist, loading default & writing.\n"));

		this->HandleDefaultScript(_("custom_crosshair"),
			_(
			"function CustomCross()\n"
			"	surface.SetDrawColor(Color(255, 255, 255))\n"
			"	surface.DrawLine(ScrW() / 2 - 10, ScrH() / 2, ScrW() / 2 + 10, ScrH() / 2)\n"
			"	surface.DrawLine(ScrW() / 2 , ScrH() / 2 - 10, ScrW() / 2 , ScrH() / 2 + 10)\n"
			"end\n"
			"hook.Add(\"HUDPaint\", \"CustomCrossHair\", CustomCross)")
		);
	}

	if (!scriptFileExists(_("dark_menu_background"))) {
		Utils::ConsolePrint(_("[Lua Script Loader] dark_menu_background doesn't exist, loading default & writing.\n"));

		this->HandleDefaultScript(_("dark_menu_background"),
			_(
			"hook.Remove( \"HUDPaint\", \"_LOKIMENUDARKBG\" )\n"
			"hook.Add( \"HUDPaint\", \"_LOKIMENUDARKBG\", function()\n"
			"	if ( Loki_IsMenuOpen() ) then\n"
			"		surface.SetDrawColor( 0, 0, 0, 150 )\n"
			"		surface.DrawRect( 0, 0, ScrW(), ScrH() )\n"
			"	end\n"
			"end )\n")
		);
	}

	Utils::ConsolePrint(_("[Lua Script Loader] Reading lua folder.\n"));
	std::vector<std::string> to_load;
	std::vector<std::string> files;
	
	read_directory(LUAS_FOLDER, &files);

	if (files.size() > 2) {
		for (size_t i = 2; i < files.size(); i++) { // skip 2 since it has "." & ".."
			auto filename = files[i];
			auto fname_split = explode(filename, '.');
			if (fname_split.size() < 3) {
				Utils::ConsolePrint(_("[Lua Script Loader] Skipping %s (size < 3).\n"), filename.c_str());
				continue;
			}

			if (fname_split[1] != std::string("loki")) {
				Utils::ConsolePrint(_("[Lua Script Loader] Skipping %s (not loki lua).\n"), filename.c_str());
				continue;
			}
			if (fname_split[2] != std::string("lua")) {
				Utils::ConsolePrint(_("[Lua Script Loader] Skipping %s (not lua file).\n"), filename.c_str());
				continue;
			}

			bool bAlreadyLoaded = !!this->GetScriptByName(fname_split[0]);

			if (bAlreadyLoaded) {
				Utils::ConsolePrint(_("[Lua Script Loader] Skipping %s (script with that name already loaded)\n"), fname_split[0].c_str());
				continue;
			}

			to_load.push_back(fname_split[0]);
		}
	}
	else {
		Utils::ConsolePrint(_("[Lua Script Loader] Script directory is empty.\n"));
	}

	for (auto it : to_load) {
		std::ifstream file_hndl(SCRIPT(it));
		std::string content((std::istreambuf_iterator<char>(file_hndl)), (std::istreambuf_iterator<char>()));
		{
			this->m_vecScriptList.push_back(new LuaScript(it, content));

			Utils::ConsolePrint(_("[Lua Script Loader] Loaded %s.loki.lua\n"), it.c_str());
		}
		file_hndl.close();
	}
}

std::vector<std::string> ScriptManager::GetScriptNameList() {
	std::vector<std::string> out;
	for (auto it : this->m_vecScriptList) {
		out.push_back(it->m_ScriptName);
	}
	return out;
}

std::vector<std::string> ScriptManager::GetScriptCodeList() {
	std::vector<std::string> out;
	for (auto it : this->m_vecScriptList) {
		out.push_back(it->m_ScriptCode);
	}
	return out;
}

const char* ScriptManager::GetScriptCodeByName(std::string name) {
	for (auto it : this->m_vecScriptList) {
		if (it->m_ScriptName == name) {
			return it->m_ScriptCode.c_str();
		}
	}

	return nullptr;
}

LuaScript* ScriptManager::GetScriptByName(std::string name) {
	for (auto it : this->m_vecScriptList) {
		if (it->m_ScriptName == name) return it;
	}

	return nullptr;
}

// if the name exists, overwrite it, if it doesn't then create a new object.
LuaScript* ScriptManager::SaveScript(std::string name, std::string code) {
	auto script_exists = !!(this->GetScriptByName(name));

	if (script_exists) {
		auto script = this->GetScriptByName(name);
		if (!script) {
			Utils::ConsolePrint(_("[LUA::SaveScript] Something has majorly fucked up....\n"));
			goto FUCK; // this should NEVER happen.
		}
		Utils::ConsolePrint(_("[LUA::SaveScript] Script already exists.. overwriting.\n"));

		script->m_ScriptCode = code;

		return script;
	}
	else {
	FUCK:
		LuaScript* new_script = new LuaScript(name, code);
		this->m_vecScriptList.push_back(new_script);
		return new_script;
	}
}