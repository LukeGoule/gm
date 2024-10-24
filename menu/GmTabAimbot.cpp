#include "GmTabAimbot.h"
#include "GMMenu.h"
#include "../core/options.h"

GmTabAimbot::GmTabAimbot(std::string s, ImFont* f) : MenuTab(TABID_AIMBOT, s, f) { }
GmTabAimbot::~GmTabAimbot() { }
void GmTabAimbot::DrawSideBar() { }

void GmTabAimbot::DrawPage() {
	ImGui::PushFont(GmMenu::Get().NormalFont);
	{
		ImGui::Checkbox("Aimbot Master Switch", &g_Options.bAimMasterEnabled);
		ImGui::Spacing();
		ImGui::Checkbox("Silent Aim", &g_Options.bAimSilent);
		ImGui::Checkbox("Nospread", &g_Options.bAimRemoveSpread);
		ImGui::Checkbox("Autoshoot", &g_Options.bAimAutofire);
		ImGui::Spacing();
		ImGui::Checkbox("Target Players", &g_Options.bAimTargetPlayers);
		ImGui::Checkbox("Target NPCs", &g_Options.bAimTargetNPCs);
		ImGui::Spacing();
		ImGui::SliderInt("Target Bone ID", &g_Options.iTargetBoneID, 0, 128);
	}
	ImGui::PopFont();
}