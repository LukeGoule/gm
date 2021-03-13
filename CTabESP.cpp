#include "CTabESP.h"
#include "options.h"

CTabESP::CTabESP(std::string s, ImFont* f) : CTab(TABID_ESP, s, f) {

}

CTabESP::~CTabESP() {

}

void CTabESP::DrawPage() {
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

void CTabESP::DrawSideBar() {

}