#pragma once

#include <string>
#include <vector>

#include "imgui/imgui.h"

class CMenuPopup;

typedef void(MenuPopupCallback_t)(CMenuPopup* _this);

class CMenuPopupManager {
	std::vector<CMenuPopup*> m_vecPopupList;
public:

	CMenuPopupManager();
	~CMenuPopupManager();

	void NewPopup(std::string Title, MenuPopupCallback_t* Callback);
	void DrawAll();
};

extern CMenuPopupManager* g_pMenuPopups;

class CMenuPopup {
	ImVec2					m_vPopupPosition;
	std::string				m_Title;
	MenuPopupCallback_t*	m_pCallback = 0;
	bool m_bIsOpen			= true;
	int  m_iID				= 0;

public:

	CMenuPopup(int id, float x, float y, std::string Title, MenuPopupCallback_t* Callback);
	~CMenuPopup();

	void Draw();
};

