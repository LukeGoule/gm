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


class CTab;

class CTabManager {

#define CTABMANAGER_CREATETAB(N, F, T) this->m_vTabs.push_back(new T(N, F));
	std::vector<CTab*> m_vTabs;

	int m_iCurrentTabID = 0;

public:

	CTabManager();
	~CTabManager();

	void	DrawPage();
	void	DrawSideBar();
	void	DrawTabLines();

	int		GetCurrentTabID();
	CTab*	GetCurrentTab();
};

class CTab {
public:
	ImFont* m_pFont = 0;
	std::string m_IconStr = "";
	int m_iID = 0;

	CTab(int ID, std::string iconstr, ImFont* font);
	~CTab();

	virtual void DrawPage() = 0;
	virtual void DrawSideBar() = 0;
};

/* Static variables needed by all tab rendering instances. */

void UpdateMenuVariables();

extern int		padding;
extern ImVec2	MainMenuSize;
extern ImVec2	ScriptMenuSize;
extern ImVec2	screenSize;
extern ImVec2	MainMenuPos;
extern ImVec2	ScriptMenuPos;
extern int		divider;
extern int		nButtons;
extern ImVec2	ButtonSize;
extern Color	ButtonColor;
extern Color	ButtonColorActive;
extern Color	ButtonColorHovered;
extern ImVec4	_ButtonColor;
extern ImVec4	_ButtonColorActive;
extern ImVec4	_ButtonColorHovered;
extern Color	TabColorActive;
extern ImVec4	_TabColorActive;
extern float	iFrameX;
extern float	iFrameY;