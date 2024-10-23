#pragma once

#include "../helpers/singleton.h"
#include "../imgui/imgui.h"
#include "../core/imgui_custom.h"
#include "../imgui/text_edit/TextEditor.h"
#include "../lua_source/lua.hpp"

#include "GmMenuTab.h"
#include "GmMenuTabManager.h"

#include <string>
#include <vector>


class GmMenu : public Singleton<GmMenu> {
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
	
	GmMenuTabManager* m_pTabs	= nullptr;

	bool m_bRunning			= true; // is the cheat running
	bool m_bMenuOpen		= true; // is the menu open
	bool m_bInitialised		= false; // is the minu ready to obtain the grain

	void Setup();
	void DrawWatermark();
	void SetupStyle();
	void Render();
};