#pragma once
#include "CTab.h"
class CTabSettings :
	public CTab
{
public:

	CTabSettings(std::string s, ImFont* f);
	~CTabSettings();

	void DrawPage();
	void DrawSideBar();
};

