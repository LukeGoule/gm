#include "xmods.h"
#include "globals.h"
#include "radiomanager.h"

#include "ProggyTiny.h"
#include "Fonts2.h"
#include "menu_icons.h"
#include "Cousine.h"
#include "droid.h"
#include "icons.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/text_edit/TextEditor.h"
#include "imgui_custom.h"

#include "sdk.h"
#include "IVEngineClient.h"
#include "ISurface.h"

#include "luaShared.h"
#include "lua_source/lua.hpp"
#include "luamanager.h"
#include "luabindings.h"
#include "obfs.h"
#include "DXDraw.h"
#include "CMenuPopup.h"

#include <vector>
#include <unordered_map>

ScriptManager*		g_pLuaScripts		= nullptr;	// luamanager.h
LuaBindings*		g_pLuaBindings		= nullptr;	// luabindings.h
CDraw*				g_pDrawHelper		= nullptr;  // DXDraw.h
CFont*				g_pWatermarkFont	= nullptr;	// DXDraw.h
RadioManager*		g_pRadio			= nullptr;	// radiomanager.h
CMenuPopupManager*	g_pMenuPopups		= nullptr;	// cmenupopup.h

void MENU_CHEATNAME::Setup() {
	this				->SetupStyle();

	g_pRadio			= new RadioManager();
	g_pWatermarkFont	= new CFont(g_pD3DDevice9, 17, "Verdana");
	g_pDrawHelper		= new CDraw(g_pD3DDevice9);
	g_pLuaScripts		= new ScriptManager();
	g_pLuaBindings		= new LuaBindings();
	g_pMenuPopups		= new CMenuPopupManager();

	ChannelsCombo		= new Combo(0, g_pRadio->GetNames());
	ChannelsCombo		->_n = g_pRadio->m_iRadioChannel;
	ChannelsCombo		->current_item = ChannelsCombo->items[g_pRadio->m_iRadioChannel];

	LuaTypeCombo		= new Combo(1, {
		_("Client"),
		_("Server"),
		_("Menu")
	});
	LuaTypeCombo		->_n = 0;
	LuaTypeCombo		->current_item = _("Client");

	ScriptCombo			= new Combo(2, g_pLuaScripts->GetScriptNameList());
	ScriptCombo			->_n = 0;
	ScriptCombo			->current_item = g_pLuaScripts->GetScriptNameList()[0];

	auto language /* lua */ = TextEditor::LanguageDefinition::Lua();
	editor = new TextEditor();
	editor->SetText(_("-- this is a lua text editor\n-- be warned, some ACs may pick up usage of this."));
	editor->SetShowWhitespaces(false);

	m_pTabs = new CTabManager();
}

void MENU_CHEATNAME::DrawWatermark() {
	std::string watermark = _(MENU_CHEATNAME_S);

	if (g_pRadio && g_pRadio->m_iRadioChannel != 0) {
		watermark += _(" | Currently playing ") + g_pRadio->GetCurrentlyPlaying()->name;
	}
	
	g_pWatermarkFont->Render(10, 10, 0xFFFFFFFF, CFont::CFont_Flags::F_DROPSHADOW, "%s", watermark.c_str());
}

void MENU_CHEATNAME::Render() {
	bool doExecute = false;
	int  type = 0;
	static bool ScriptWindowSizeSet = false;

	ImGui::GetIO().MouseDrawCursor	= this->m_bMenuOpen;  // yes it's vsyncing no i don't care
	ChannelsCombo->items			= g_pRadio->GetNames();
	ScriptCombo->items				= g_pLuaScripts->GetScriptNameList();

	if (this->m_bMenuOpen && MENU_CHEATNAME::Get().m_bRunning) {
		g_pMenuPopups->DrawAll();

		ImGui::SetNextWindowSize(ScriptMenuSize, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2((screenSize.x / 2) - (ScriptMenuSize.x / 2), (screenSize.y / 2) - (ScriptMenuSize.x / 2)));

		ImGui::Begin(_("##new_menu"), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			ImGui::BeginChildFrame(1212, ImVec2((ScriptMenuSize.x / divider) - (ImGui::GetStyle().WindowPadding.x * 2), ScriptMenuSize.y));
			{
				ImGui::PushItemWidth(ScriptMenuSize.x / divider);
				{
					this->m_pTabs->DrawSideBar();
				}
				ImGui::PopItemWidth();
			}
			ImGui::EndChildFrame();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();

			ImGui::SameLine(0, 0);
			ImGui::PushFont(NormalFont);
			{
				this->m_pTabs->DrawPage();
			}
			ImGui::PopFont();

			this->m_pTabs->DrawTabLines();
		}

		ImGui::End();
	}
}

void MENU_CHEATNAME::SetupStyle() {
	ImGuiStyle * style = &ImGui::GetStyle();

	style->WindowPadding		= ImVec2(0, 0);
	style->WindowRounding		= 0.0f;
	style->FramePadding			= ImVec2(3, 3);
	style->FrameRounding		= 0.0f;
	style->ItemSpacing			= ImVec2(5, 5);
	style->ItemInnerSpacing		= ImVec2(8, 6);
	style->IndentSpacing		= 25.0f;
	style->ScrollbarSize		= 5.0f;
	style->ScrollbarRounding	= 0.0f;
	style->GrabMinSize			= 1.0f;
	style->GrabRounding			= 0.0f;

	auto clr = o.vecMenuColorScheme;

	auto ttff = 10.f / 255.f; // ten two five five.
	auto ButtonColor			= clr;
	auto ButtonColorActive		= ImVec4(clr.x - ttff, clr.y - ttff, clr.z - ttff, clr.w);
	auto ButtonColorHovered		= ImVec4(clr.x + ttff, clr.y + ttff, clr.z + ttff, clr.w);

	style->Colors[ImGuiCol_FrameBgActive]	= ButtonColorActive;
	style->Colors[ImGuiCol_FrameBgHovered]	= ButtonColorHovered;
	style->Colors[ImGuiCol_CheckMark]		= clr;
	style->Colors[ImGuiCol_FrameBg]			= clr;
	style->Colors[ImGuiCol_Button]			= ButtonColor;
	style->Colors[ImGuiCol_ButtonHovered]	= ButtonColorHovered;
	style->Colors[ImGuiCol_ButtonActive]	= ButtonColorActive;

	ImGuiIO& io		= ImGui::GetIO();
	
	DefaultFont		= io.Fonts->AddFontDefault();
	NormalFont		= io.Fonts->AddFontFromMemoryCompressedTTF((void*)Ubuntu_compressed_data,		Ubuntu_compressed_size,					13);
	TitleFont		= io.Fonts->AddFontFromMemoryCompressedTTF((void*)VerminVibes_compressed_data,	VerminVibes_compressed_size,			18);
	SubTitleFont	= io.Fonts->AddFontFromMemoryCompressedTTF((void*)VerminVibes_compressed_data,	VerminVibes_compressed_size,			15);
	IconFont		= io.Fonts->AddFontFromMemoryCompressedTTF((void*)IconsForTabs_compressed_data, IconsForTabs_compressed_size,			35.f);
	IconFont2		= io.Fonts->AddFontFromMemoryCompressedTTF((void*)icons_compressed_data,		icons_compressed_size,					35.f);
	Cousine			= io.Fonts->AddFontFromMemoryCompressedTTF((void*)Cousine_compressed_data,		Cousine_compressed_size,				30);
	Droid			= io.Fonts->AddFontFromMemoryCompressedTTF((void*)Droid_compressed_data,		Droid_compressed_size,					30);
	CodeFont		= io.Fonts->AddFontFromFileTTF(_("C:\\Windows\\Fonts\\consola.ttf"), 13.5f); // Consolas

	this->m_bInitialised = true;
}