#pragma once

#include "GmMenuTab.h"

class GmTabScripting : public MenuTab {
public:
	GmTabScripting(std::string s, ImFont* f);
	~GmTabScripting();

	void DrawPage();
	void DrawSideBar();
};

