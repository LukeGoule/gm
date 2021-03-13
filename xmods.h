#pragma once

#include "singleton.h"
#include "imgui/imgui.h"
#include "imgui_custom.h"
#include "imgui/text_edit/TextEditor.h"
#include "lua_source/lua.hpp"

#include "CTab.h"

#include <string>
#include <vector>

/*
17/6/2020 - Refactor menu code:
			why would I have a class (Loki) which isn't used for anything except being a pain in the backside for manipulating the menu?
			totally stupid, fixed. also carpal tunnel in my right hand smh
*/



// not so you can paste, it's incase i decide to release this or whatever. 
// someone on UC took the cheat name "Loki" for their lua cheat dispite it 
// being me and moonman building the original loki.red smh
#define MENU_CHEATNAME		xmods
#define MENU_CHEATNAME_S	"xmods"

extern const char* GetLoaderUserName();
extern const char* GetLoaderUserPassword();
extern const char* GetLoaderUserKey();

class MENU_CHEATNAME : public Singleton<MENU_CHEATNAME> {
public:
	/* Fonts, some are unused. */
	ImFont* NormalFont		= 0;
	ImFont* TitleFont		= 0;
	ImFont* SubTitleFont	= 0;
	ImFont* DefaultFont		= 0;
	ImFont* IconFont		= 0;
	ImFont* Cousine			= 0;
	ImFont* Droid			= 0;
	ImFont* IconFont2		= 0;
	ImFont* CodeFont		= 0;

	/* Comboboxes */
	Combo* ChannelsCombo;
	Combo* LuaTypeCombo;
	Combo* ScriptCombo;

	TextEditor* editor;
	
	CTabManager* m_pTabs	= nullptr;

	bool m_bRunning			= true; // is the cheat running
	bool m_bMenuOpen		= true; // is the menu open
	bool m_bInitialised		= false; // is the minu ready to obtain the grain

	void Setup();
	void DrawWatermark();
	void SetupStyle();
	void Render();
};