#include "CTabSettings.h"
#include "options.h"
#include "xmods.h"
#include "CMenuPopup.h"

CTabSettings::CTabSettings(std::string s, ImFont* f) : CTab(TABID_SETTINGS, s, f) {

}

CTabSettings::~CTabSettings() {

}

std::string simplecutoff(std::string s, int maxlen) {
	std::string out = "";

	if (s.size() > maxlen) {
		out = s.substr(0, maxlen);
		out += "..";
		return out;
	}
	else {
		return s;
	}
}

void CTabSettings::DrawPage() {
	ImGui::Checkbox(_("Cheat Running"), &xmods::Get().m_bRunning);
	ImGui::Checkbox(_("Output log to ingame console"), &o.bOutputLogToConsole);
	ImGui::Checkbox(_("Draw Watermark"), &o.bDrawWaterMark);
	ImGui::PushItemWidth(100.f);
	ImGui::ColorEdit3("Menu Theme Color", &o.vecMenuColorScheme.x);
	ImGui::PopItemWidth();

	ImGui::Spacing();

	if (ImGui::Button("Test Popup")) {
		g_pMenuPopups->NewPopup("test", 
			[](CMenuPopup* thisptr) {
				ImGui::Text("HI!");
			});
	}
}

void CTabSettings::DrawSideBar() {

}