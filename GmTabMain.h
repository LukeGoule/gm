#pragma once

#include "GmMenuTab.h"

class GmTabMain : public MenuTab {
public:
	GmTabMain(std::string s, ImFont* f);
	~GmTabMain();

	void DrawPage();
	void DrawSideBar();
};

