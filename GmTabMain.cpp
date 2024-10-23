#include "GmTabMain.h"
#include "GmMenuTab.h"
#include "GmMenu.h"
#include "options.h"

GmTabMain::GmTabMain(std::string s, ImFont* f) : MenuTab(TABID_MAINMENU, s, f) { }
GmTabMain::~GmTabMain() { }
void GmTabMain::DrawSideBar() { }

void GmTabMain::DrawPage() {
	ImGui::BeginChildFrame(1238019, ImVec2(iFrameX / 2, iFrameY - 50));
	{
		ImGui::PushFont(GmMenu::Get().Droid);
		{
			ImGui::LabelText("", std::string(_("Welcome back ")).c_str());
		}
		ImGui::PopFont();

		ImGui::PushFont(GmMenu::Get().SubTitleFont);
		{
			ImGui::LabelText("", _("Gameplay Options"));
		}
		ImGui::PopFont();

		ImGui::Checkbox(_("Bunnyhop"), &o.bBhop);
		ImGui::Checkbox(_("No Visual Kick"), &o.bNoKick);
		ImGui::Checkbox(_("Draw Viewmodel"), &o.bDrawViewmodel);
	}
	ImGui::EndChildFrame();

	ImGui::SameLine(0, 0);

	ImGui::BeginChildFrame(342323, ImVec2(iFrameX / 2, iFrameY - 50));
	{
		ImGui::PushFont(GmMenu::Get().SubTitleFont);
		{
			ImGui::LabelText("", _("Side #2"));
		}
		ImGui::PopFont();
	}
	ImGui::EndChildFrame();
}