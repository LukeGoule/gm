#pragma once

#include <vector>

class MenuTab;

class GmMenuTabManager {

#define CTABMANAGER_CREATETAB(N, F, T) this->m_vTabs.push_back(new T(N, F));
	std::vector<MenuTab*> m_vTabs;

	int m_iCurrentTabID = 0;

public:

	GmMenuTabManager();
	~GmMenuTabManager();

	void	DrawPage();
	void	DrawSideBar();
	void	DrawTabLines();

	int		GetCurrentTabID();
	MenuTab* GetCurrentTab();
};

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