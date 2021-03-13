#include "CMenuPopup.h"

CMenuPopupManager::CMenuPopupManager() {

}

CMenuPopupManager::~CMenuPopupManager() {

}

void CMenuPopupManager::NewPopup(std::string Title, MenuPopupCallback_t* Callback) {
	// TODO: unoccupied location for new popup.
	this->m_vecPopupList.push_back(new CMenuPopup(rand(), 100, 100, Title, Callback));
}

void CMenuPopupManager::DrawAll() {
	for (auto Popup : this->m_vecPopupList) {
		Popup->Draw();
	}
}

CMenuPopup::CMenuPopup(int id, float x, float y, std::string Title, MenuPopupCallback_t* Callback) {
	this->m_vPopupPosition = ImVec2(x, y);
	this->m_Title = Title;
	this->m_pCallback = Callback;

	this->m_vPopupPosition = ImVec2(x, y);
}

CMenuPopup::~CMenuPopup() {

}

void CMenuPopup::Draw() {
	//ImGui::SetNextWindowPos(this->m_vPopupPosition);
	ImGui::SetNextWindowSize({ 300.f, 200.f });
	ImGui::PushID(this->m_iID);
	ImGui::Begin(this->m_Title.c_str());
	{
		this->m_pCallback(this);
	}
	ImGui::End();
	ImGui::PopID();
}