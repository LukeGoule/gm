#include "GmMenuTab.h"

MenuTab::MenuTab(int ID, std::string iconstr, ImFont* font) {
	this->m_iID = ID;
	this->m_IconStr	= iconstr;
	this->m_pFont	= font;
}

MenuTab::~MenuTab() {}

