#include "CTabAimbot.h"
#include "xmods.h"
#include "options.h"

CTabAimbot::CTabAimbot(std::string s, ImFont* f) : CTab(TABID_AIMBOT, s, f) {

}

CTabAimbot::~CTabAimbot() {

}

void CTabAimbot::DrawPage() {
	ImGui::PushFont(xmods::Get().NormalFont);
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

void CTabAimbot::DrawSideBar() {}