#pragma once

#include <string>
#include <vector>

#include "../imgui/imgui.h"

class GmMenuPopup;

typedef void(MenuPopupCallback_t)(GmMenuPopup* _this);

class GmMenuPopupManager {
	std::vector<GmMenuPopup*> m_vecPopupList;
public:

	GmMenuPopupManager();
	~GmMenuPopupManager();

	void NewPopup(std::string Title, MenuPopupCallback_t* Callback);
	void DrawAll();
};

extern GmMenuPopupManager* g_pMenuPopups;

class GmMenuPopup {
	ImVec2					m_vPopupPosition;
	std::string				m_Title;
	MenuPopupCallback_t*	m_pCallback = 0;
	bool m_bIsOpen			= true;
	int  m_iID				= 0;

public:

	GmMenuPopup(int id, float x, float y, std::string Title, MenuPopupCallback_t* Callback);
	~GmMenuPopup();

	void Draw();
};

