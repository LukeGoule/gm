#pragma once
#include "CTab.h"
class CTabESP :
	public CTab
{
public:

	CTabESP(std::string s, ImFont* f);
	~CTabESP();

	void DrawPage();
	void DrawSideBar();

};

