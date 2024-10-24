#include "GmTabSettings.h"
#include "../core/options.h"
#include "GMMenu.h"
#include "GmMenuPopup.h"

GmTabSettings::GmTabSettings(std::string s, ImFont* f) : MenuTab(TABID_SETTINGS, s, f) { }
GmTabSettings::~GmTabSettings() { }
void GmTabSettings::DrawSideBar() { }

void GmTabSettings::DrawPage() {
	ImGui::Checkbox(_("Cheat Running"), &GmMenu::Get().m_bRunning);
	ImGui::Checkbox(_("Output log to ingame console"), &g_Options.bOutputLogToConsole);
	ImGui::Checkbox(_("Draw Watermark"), &g_Options.bDrawWaterMark);
	
	ImGui::PushItemWidth(100.f);
	{
		ImGui::ColorEdit3("Menu Theme Color", &g_Options.vecMenuColorScheme.x);
	}
	ImGui::PopItemWidth();
}