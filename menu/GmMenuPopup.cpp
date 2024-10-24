#include "GmMenuPopup.h"

#include "../../imgui/text_edit/TextEditor.h"

GmMenuPopupManager::GmMenuPopupManager() {

}

GmMenuPopupManager::~GmMenuPopupManager() {

}

void GmMenuPopupManager::NewPopup(std::string Title, MenuPopupCallback_t* Callback) {
	this->m_vecPopupList.push_back(new GmMenuPopup(rand(), 100, 100, Title, Callback));
}

void GmMenuPopupManager::DrawAll() {
	for (const auto Popup : this->m_vecPopupList) {
		Popup->Draw();
	}
}

GmMenuPopup::GmMenuPopup(int id, float x, float y, std::string Title, MenuPopupCallback_t* Callback) {
	this->m_vPopupPosition = ImVec2(x, y);
	this->m_Title = Title;
	this->m_pCallback = Callback;

	this->m_vPopupPosition = ImVec2(x, y);
}

GmMenuPopup::~GmMenuPopup() {

}

void GmMenuPopup::Draw() {
	ImGui::PushID(this->m_iID);
	ImGui::Begin(this->m_Title.c_str());
	{
		this->m_pCallback(this);
	}
	ImGui::End();
	ImGui::PopID();
}

GmMenuLuaEditorPopup::GmMenuLuaEditorPopup(int id, std::string Title, std::string Code) : GmMenuPopup(id, 100, 100, Title, nullptr)
{
	this->m_pEditor = new TextEditor;
	this->m_pEditor->SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
	this->m_pEditor->SetText(Code);
}

void GmMenuLuaEditorPopup::Draw()
{
	ImGui::PushID(this->m_iID);
	ImGui::Begin(this->m_Title.c_str());
	{
		this->m_pEditor->Render(this->m_Title.c_str());
	}
	ImGui::End();
	ImGui::PopID();
}