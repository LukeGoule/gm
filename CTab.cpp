#include "CTab.h"

#include "CTabMain.h"
#include "CTabAimbot.h"
#include "CTabScripting.h"
#include "CTabESP.h"
#include "CTabSettings.h"

#include "xmods.h"
#include "globals.h" // SilentCrash()

int		padding;
ImVec2	MainMenuSize;
ImVec2	ScriptMenuSize;
ImVec2	screenSize;
ImVec2	MainMenuPos;
ImVec2	ScriptMenuPos;
int		divider;
int		nButtons;
ImVec2	ButtonSize;
Color	ButtonColor;
Color	ButtonColorActive;
Color	ButtonColorHovered;
ImVec4	_ButtonColor;
ImVec4	_ButtonColorActive;
ImVec4	_ButtonColorHovered;
Color	TabColorActive;
ImVec4	_TabColorActive;
float	iFrameX;
float	iFrameY;

/* CTabManager for helping draw the tab list. (keeps the line count down in loki.cpp */

CTabManager::CTabManager() {
	auto pTitleFont = xmods::Get().TitleFont;
	auto pIconFont1 = xmods::Get().IconFont;
	auto pIconFont2 = xmods::Get().IconFont2;

	if (!pTitleFont || !pIconFont1 || !pIconFont2) {
		MessageBoxA(0, "Menu fonts not initialised!", "", 0);
		SilentCrash();
	}


	CTABMANAGER_CREATETAB(_(MENU_CHEATNAME_S),	pTitleFont, CTabMain);			// Main Menu				- Text for icon
	CTABMANAGER_CREATETAB(_("A"),				pIconFont2, CTabAimbot);		// Aimbot					- Headshot icon
	CTABMANAGER_CREATETAB(_("F"),				pIconFont2, CTabScripting);		// Scripting				- Notepad icon
	CTABMANAGER_CREATETAB(_("I"),				pIconFont1, CTabESP);			// ESP						- Snapshot icon
	CTABMANAGER_CREATETAB(_("G"),				pIconFont2, CTabSettings);		// Menu Settings / CFG		- Cogs icon
}

CTabManager::~CTabManager() {

}

void CTabManager::DrawSideBar() {
	UpdateMenuVariables(); // called only once per frame

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _ButtonColorHovered);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, _ButtonColorActive);
	ImGui::PushStyleColor(ImGuiCol_Button, _ButtonColor);
	{
		for (auto Tab : this->m_vTabs) {
			ImGui::PushFont(Tab->m_pFont);
			{
				if (this->m_iCurrentTabID == Tab->m_iID) {
					ImGui::PushStyleColor(ImGuiCol_Button, _TabColorActive);
					{
						ImGui::Button(Tab->m_IconStr.c_str(), ButtonSize);
					}
					ImGui::PopStyleColor();
				}
				else {
					if (ImGui::Button(_(Tab->m_IconStr.c_str()), ButtonSize)) this->m_iCurrentTabID = Tab->m_iID;
				}
			}
			ImGui::PopFont();
		}
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void CTabManager::DrawPage() {
	ImGui::PushStyleColor(ImGuiCol_FrameBg, COLOR2IMVEC(Color(30, 30, 30, 255)));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
	{
		ImGui::BeginChildFrame(123091, ImVec2(iFrameX, iFrameY));
		{
			for (auto Tab : this->m_vTabs) {
				if (this->m_iCurrentTabID == Tab->m_iID) {
					Tab->DrawPage();
				}
			}
		}
		ImGui::EndChildFrame();
	}
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

}

void CTabManager::DrawTabLines() {
	auto window = ImGui::GetCurrentWindow();
	auto color = ImGui::GetColorU32(COLOR2IMVEC(50, 50, 50, 255));
	auto dl = ImGui::GetForegroundDrawList();

	switch (this->m_iCurrentTabID) {
	case TABID_MAINMENU:
		// horizontal
		dl->AddLine(
			ImVec2(
				window->Pos.x + 1,
				window->Pos.y + ButtonSize.y
			),
			ImVec2(
				window->Pos.x + ButtonSize.x,
				window->Pos.y + ButtonSize.y
			),
			color, .25f);

		// vertical
		dl->AddLine(ImVec2(
			window->Pos.x + ButtonSize.x,
			window->Pos.y + ButtonSize.y
		), ImVec2(
			window->Pos.x + ButtonSize.x,
			window->Pos.y + window->Size.y),
			color, .25f);
		break;
	case TABID_AIMBOT:
	case TABID_SCRIPTING:
	case TABID_ESP:
		// horizontal
		dl->AddLine(
			ImVec2(
				window->Pos.x + 1,
				window->Pos.y + (ButtonSize.y * (this->m_iCurrentTabID))
			),
			ImVec2(
				window->Pos.x + ButtonSize.x,
				window->Pos.y + (ButtonSize.y * this->m_iCurrentTabID)
			),
			color, .25f);

		dl->AddLine(
			ImVec2(
				window->Pos.x + 1,
				window->Pos.y + (ButtonSize.y * (this->m_iCurrentTabID + 1))
			),
			ImVec2(
				window->Pos.x + ButtonSize.x,
				window->Pos.y + (ButtonSize.y * (this->m_iCurrentTabID + 1))
			),
			color, .25f);


		// vertical

		// top vertical 
		dl->AddLine(ImVec2(
			window->Pos.x + ButtonSize.x,
			window->Pos.y
		), ImVec2(
			window->Pos.x + ButtonSize.x,
			window->Pos.y + (ButtonSize.y * (this->m_iCurrentTabID)) + 1
		),
			color, .25f);

		// bottom vertical 
		dl->AddLine(ImVec2(
			window->Pos.x + ButtonSize.x,
			window->Pos.y + (ButtonSize.y * (this->m_iCurrentTabID + 1))
		), ImVec2(
			window->Pos.x + ButtonSize.x,
			window->Pos.y + window->Size.y),
			color, .25f);

		break;
	case TABID_SETTINGS:
		// horizontal
		dl->AddLine(
			ImVec2(
				window->Pos.x + 1,
				window->Pos.y + (ButtonSize.y * (this->m_iCurrentTabID))
			),
			ImVec2(
				window->Pos.x + ButtonSize.x,
				window->Pos.y + (ButtonSize.y * (this->m_iCurrentTabID))
			),
			color, .25f);

		// vertical 
		dl->AddLine(ImVec2(
			window->Pos.x + ButtonSize.x,
			window->Pos.y
		), ImVec2(
			window->Pos.x + ButtonSize.x,
			window->Pos.y + window->Size.y - (ButtonSize.y - 1)),
			color, .25f);
		break;
	}
}

int	CTabManager::GetCurrentTabID() {
	return this->m_iCurrentTabID;
}

CTab* CTabManager::GetCurrentTab() {
	try {
		return this->m_vTabs[this->m_iCurrentTabID];
	}
	catch (std::exception e) {
		return nullptr;
	}
}


/* CTab Base Class */

CTab::CTab(int ID, std::string iconstr, ImFont* font) {
	this->m_iID = ID;
	this->m_IconStr	= iconstr;
	this->m_pFont	= font;
}

CTab::~CTab() {

}

void UpdateMenuVariables() {

	padding = 30;
	MainMenuSize = ImVec2(320, 500);
	ScriptMenuSize = ImVec2(800, 500);
	screenSize = ImGui::GetIO().DisplaySize;
	MainMenuPos = ImVec2(
		(screenSize.x / 2) - ((MainMenuSize.x + ScriptMenuSize.x + 30) / 2),
		(screenSize.y / 2) - (MainMenuSize.y / 2));
	ScriptMenuPos = ImVec2(
		MainMenuPos.x + MainMenuSize.x + 30,
		(screenSize.y / 2) - (ScriptMenuSize.y / 2));
	divider = 10;
	nButtons = 5;
	ButtonSize = ImVec2((ScriptMenuSize.x / divider), (ScriptMenuSize.y / nButtons));
	ButtonColor = Color(15, 15, 15, 255);
	ButtonColorActive = Color(30, 30, 30, 255);
	ButtonColorHovered = Color(25, 25, 25, 255);
	TabColorActive = Color(30, 30, 30, 255);
	_TabColorActive = COLOR2IMVEC(TabColorActive);
	_ButtonColor = COLOR2IMVEC(ButtonColor);
	_ButtonColorActive = COLOR2IMVEC(ButtonColorActive);
	_ButtonColorHovered = COLOR2IMVEC(ButtonColorHovered);
	iFrameX = ((ScriptMenuSize.x / divider) * (divider - 1)) - (ImGui::GetStyle().WindowPadding.x * 2);
	iFrameY = ScriptMenuSize.y;
}