#pragma once

#include "imgui/imgui.h"
#include "imgui_custom.h"
#include "imgui/imgui_internal.h"

#include "Color.h"
#include <vector>

#define TABID_MAINMENU	0
#define TABID_AIMBOT	1
#define TABID_SCRIPTING 2
#define TABID_ESP		3
#define TABID_SETTINGS	4

class MenuTab {
public:
	ImFont* m_pFont = 0;
	std::string m_IconStr = "";
	int m_iID = 0;

	MenuTab(int ID, std::string iconstr, ImFont* font);
	~MenuTab();

	virtual void DrawPage() = 0;
	virtual void DrawSideBar() = 0;
};