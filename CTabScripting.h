#pragma once
#include "CTab.h"
class CTabScripting :
	public CTab
{
public:

	CTabScripting(std::string s, ImFont* f);
	~CTabScripting();

	void DrawPage();
	void DrawSideBar();

};

