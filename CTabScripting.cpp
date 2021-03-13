#include "CTabScripting.h"
#include "xmods.h"
#include "luamanager.h"
#include "luabindings.h"
#include "IVEngineClient.h"
#include "luaShared.h"
#include "globals.h"

CTabScripting::CTabScripting(std::string s, ImFont* f) : CTab(TABID_SCRIPTING, s, f) {

}

CTabScripting::~CTabScripting() {

}

void CTabScripting::DrawPage() {
	int type = 0;
	bool doExecute = false;

	ImGui::PushFont(xmods::Get().CodeFont);
	{
		xmods::Get().editor->Render(_("LUA Scripting"), ImVec2(iFrameX - 10, iFrameY - 100));
	}
	ImGui::PopFont();

	ImGui::PushFont(xmods::Get().NormalFont);
	{
		ImGui::BeginChildFrame(12380192323, ImVec2(iFrameX / 2, 75));
		{
			ImGui::PushItemWidth(150.f);
			{
				type = xmods::Get().LuaTypeCombo->Draw(_("Script Type"));
			}
			ImGui::SameLine();
			if (ImGui::Button(_("Run Script"))) {
				doExecute = true;
			}
		}
		ImGui::EndChildFrame();

		ImGui::SameLine(0, 0);

		ImGui::BeginChildFrame(12380123239, ImVec2(iFrameX / 2, 75));
		{
			auto script_id = 0;
			ImGui::PushItemWidth(150.f);
			{
				script_id = xmods::Get().ScriptCombo->Draw("");
			}
			ImGui::PopItemWidth();
			ImGui::SameLine(0, 0);

			if (ImGui::Button(_("Load"))) {
				xmods::Get().editor->SetText(g_pLuaScripts->m_vecScriptList[script_id]->m_ScriptCode);
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
					auto code = xmods::Get().editor->GetText();
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
			g_pLuaBindings->ExecString(LUA, xmods::Get().editor->GetText());
		fucksake: {

			}
		}
		else {
			// ok
		}
	}
}

void CTabScripting::DrawSideBar() {}