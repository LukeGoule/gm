#pragma once

#include "GmMenuTab.h"

class GmTabAimbot : public MenuTab {
public:
	GmTabAimbot(std::string s, ImFont* f);
	~GmTabAimbot();

	void DrawPage();
	void DrawSideBar();
};

