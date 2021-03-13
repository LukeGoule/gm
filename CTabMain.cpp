#include "CTabMain.h"
#include "CTab.h"
#include "xmods.h"
#include "radiomanager.h"
#include "options.h"

CTabMain::CTabMain(std::string s, ImFont* f) : CTab(TABID_MAINMENU, s, f) {

}

CTabMain::~CTabMain() {

}

void CTabMain::DrawPage() {
	ImGui::BeginChildFrame(1238019, ImVec2(iFrameX / 2, iFrameY - 50));
	{
		ImGui::PushFont(xmods::Get().Droid);
		{
			ImGui::LabelText("", std::string(std::string(_("Welcome back ")) + GetLoaderUserName()).c_str());
		}
		ImGui::PopFont();

		ImGui::PushFont(xmods::Get().SubTitleFont);
		{
			ImGui::LabelText("", _("Gameplay Options"));
		}
		ImGui::PopFont();

		ImGui::Checkbox(_("Bunnyhop"), &o.bBhop);
		ImGui::Checkbox(_("No Visual Kick"), &o.bNoKick);
		ImGui::Checkbox(_("Draw Viewmodel"), &o.bDrawViewmodel);

		ImGui::PushFont(xmods::Get().SubTitleFont);
		{
			ImGui::LabelText("", _("Radio Options"));
		}
		ImGui::PopFont();

		g_pRadio->m_iRadioChannel = xmods::Get().ChannelsCombo->Draw(_("Radio Channel"));
		CustomSliderInt(_("Radio Volume"), &g_pRadio->m_iRadioVolumePercent, 0, 100, _("%d%"));
	}
	ImGui::EndChildFrame();

	ImGui::SameLine(0, 0);

	ImGui::BeginChildFrame(342323, ImVec2(iFrameX / 2, iFrameY - 50));
	{
		ImGui::PushFont(xmods::Get().SubTitleFont);
		{
			ImGui::LabelText("", _("Side #2"));
		}
		ImGui::PopFont();
	}
	ImGui::EndChildFrame();
}

void CTabMain::DrawSideBar() {

}