#pragma once

#include "GmMenuTab.h"

class GmTabESP : public MenuTab
{
public:
	GmTabESP(std::string s, ImFont* f);
	~GmTabESP();

	void DrawPage();
	void DrawSideBar();
};