#pragma once

#include "GmMenuTab.h"

class GmTabSettings : public MenuTab
{
public:

	GmTabSettings(std::string s, ImFont* f);
	~GmTabSettings();

	void DrawPage();
	void DrawSideBar();
};

