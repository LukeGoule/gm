#pragma once

#include <string>
#include <vector>

#include "../imgui/imgui.h"

class GmMenuPopup;
class TextEditor;

typedef void(MenuPopupCallback_t)(GmMenuPopup* _this);

class GmMenuPopupManager {
public:

	std::vector<GmMenuPopup*> m_vecPopupList;

	GmMenuPopupManager();
	~GmMenuPopupManager();

	void NewPopup(std::string Title, MenuPopupCallback_t* Callback);
	void DrawAll();
};

extern GmMenuPopupManager* g_pMenuPopups;

class GmMenuPopup 
{
protected:

	ImVec2					m_vPopupPosition;
	std::string				m_Title;
	MenuPopupCallback_t*	m_pCallback = 0;
	bool m_bIsOpen			= true;
	int  m_iID				= 0;

public:

	GmMenuPopup(int id, float x, float y, std::string Title, MenuPopupCallback_t* Callback);
	~GmMenuPopup();

	virtual void Draw();
};


class GmMenuLuaEditorPopup: public GmMenuPopup
{
public:
	GmMenuLuaEditorPopup(int id, std::string Title, std::string Code);

	TextEditor* m_pEditor;

	void Draw() override;
};
