#pragma once
#include "CTab.h"

class CTabMain : public CTab {
public:

	CTabMain(std::string s, ImFont* f);
	~CTabMain();

	void DrawPage();
	void DrawSideBar();
};

