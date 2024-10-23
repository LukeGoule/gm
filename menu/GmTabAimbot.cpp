#include "GmTabAimbot.h"
#include "GMMenu.h"
#include "../core/options.h"

GmTabAimbot::GmTabAimbot(std::string s, ImFont* f) : MenuTab(TABID_AIMBOT, s, f) { }
GmTabAimbot::~GmTabAimbot() { }
void GmTabAimbot::DrawSideBar() { }

void GmTabAimbot::DrawPage() {
	ImGui::PushFont(GmMenu::Get().NormalFont);
	{
		ImGui::Checkbox("Aimbot Master Switch", &o.bAimMasterEnabled);
		ImGui::Spacing();
		ImGui::Checkbox("Silent Aim", &o.bAimSilent);
		ImGui::Checkbox("Nospread", &o.bAimRemoveSpread);
		ImGui::Checkbox("Autoshoot", &o.bAimAutofire);
		ImGui::Spacing();
		ImGui::Checkbox("Target Players", &o.bAimTargetPlayers);
		ImGui::Checkbox("Target NPCs", &o.bAimTargetNPCs);
		ImGui::Spacing();
		ImGui::SliderInt("Target Bone ID", &o.iTargetBoneID, 0, 128);
	}
	ImGui::PopFont();
}