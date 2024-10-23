#include "GmTabESP.h"
#include "../core/options.h"

GmTabESP::GmTabESP(std::string s, ImFont* f) : MenuTab(TABID_ESP, s, f) { }
GmTabESP::~GmTabESP() { }
void GmTabESP::DrawSideBar() { }

void GmTabESP::DrawPage() {
	ImGui::Checkbox("ESP Master Switch", &o.bESPMasterEnabled);
	ImGui::SliderInt("ESP Render Distance", &o.iESPMaxDrawDist, 100, 50000);

	ImGui::Spacing();

	ImGui::Checkbox("ESP Players", &o.bESPShowPlayers);
	ImGui::Checkbox("ESP NPCs", &o.bESPShowNPCs);
	ImGui::Checkbox("ESP Weapons", &o.bESPShowWeapons);

	ImGui::Spacing();

	ImGui::Checkbox("ESP Boxes", &o.bESPDrawBoxes);
	ImGui::Checkbox("ESP Class Names", &o.bESPDrawClassName);
	ImGui::Checkbox("ESP Player Names", &o.bESPDrawPlayerNames);
}