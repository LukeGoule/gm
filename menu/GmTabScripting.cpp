#include "GmTabScripting.h"
#include "GmMenu.h"

#include "../core/luamanager.h"
#include "../core/luabindings.h"
#include "../core/globals.h"

#include "../sdk/IVEngineClient.h"
#include "../gmod/luaShared.h"

GmTabScripting::GmTabScripting(std::string s, ImFont* f) : MenuTab(TABID_SCRIPTING, s, f) { }
GmTabScripting::~GmTabScripting() { }
void GmTabScripting::DrawSideBar() {}

void GmTabScripting::DrawPage() {
	int type = 0;
	bool doExecute = false;

	ImGui::PushFont(GmMenu::Get().CodeFont);
	{
		GmMenu::Get().editor->Render(_("LUA Scripting"), ImVec2(iFrameX - 10, iFrameY - 100));
	}
	ImGui::PopFont();

	ImGui::PushFont(GmMenu::Get().NormalFont);
	{
		ImGui::BeginChildFrame(0x13371337, ImVec2(iFrameX / 2, 75));
		{
			ImGui::PushItemWidth(150.f);
			{
				type = GmMenu::Get().LuaTypeCombo->Draw(_("Script Type"));
			}
			ImGui::SameLine();
			if (ImGui::Button(_("Run Script"))) {
				doExecute = true;
			}
		}
		ImGui::EndChildFrame();

		ImGui::SameLine(0, 0);

		ImGui::BeginChildFrame(0x13371338, ImVec2(iFrameX / 2, 75));
		{
			auto script_id = 0;
			ImGui::PushItemWidth(150.f);
			{
				script_id = GmMenu::Get().ScriptCombo->Draw("");
			}
			ImGui::PopItemWidth();
			ImGui::SameLine(0, 0);

			if (ImGui::Button(_("Load"))) {
				GmMenu::Get().editor->SetText(g_pLuaScripts->m_vecScriptList[script_id]->m_ScriptCode);
			}

			static char script_name[128];
			ImGui::PushItemWidth(150.f);
			{
				CustomTextEntry("", script_name, 128);
			}
			ImGui::PopItemWidth();
			ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
			if (ImGui::Button(_("Save Script As"))) {
				if (strlen(script_name) > 0) {
					auto code = GmMenu::Get().editor->GetText();
					g_pLuaScripts->SaveScript(std::string(script_name), code);
				}
			}
		}
		ImGui::EndChildFrame();
	}
	ImGui::PopFont();

	if (doExecute) {
		if ((g_pEngineClient->IsInGame())) {

			lua::ILuaInterface* LUA = g_pLuaShared->GetLuaInterface(type);
			if (!LUA) {
				// FUCK
				goto fucksake;
			}
			g_pLuaBindings->ExecString(LUA, GmMenu::Get().editor->GetText());
		fucksake: {

			}
		}
		else {
			// ok
		}
	}
}