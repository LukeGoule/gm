#pragma once
#include "CTab.h"
class CTabAimbot :
	public CTab
{
public:

	CTabAimbot(std::string s, ImFont* f);
	~CTabAimbot();

	void DrawPage();
	void DrawSideBar();

};

