#include "GmTabESP.h"
#include "../core/options.h"

GmTabESP::GmTabESP(std::string s, ImFont* f) : MenuTab(TABID_ESP, s, f) { }
GmTabESP::~GmTabESP() { }
void GmTabESP::DrawSideBar() { }

void GmTabESP::DrawPage() {
	ImGui::Checkbox("ESP Master Switch", &g_Options.bESPMasterEnabled);
	ImGui::SliderInt("ESP Render Distance", &g_Options.iESPMaxDrawDist, 100, 50000);

	ImGui::Spacing();

	ImGui::Checkbox("ESP Players", &g_Options.bESPShowPlayers);
	ImGui::Checkbox("ESP NPCs", &g_Options.bESPShowNPCs);
	ImGui::Checkbox("ESP Weapons", &g_Options.bESPShowWeapons);

	ImGui::Spacing();

	ImGui::Checkbox("ESP Boxes", &g_Options.bESPDrawBoxes);
	ImGui::Checkbox("ESP Class Names", &g_Options.bESPDrawClassName);
	ImGui::Checkbox("ESP Player Names", &g_Options.bESPDrawPlayerNames);
}