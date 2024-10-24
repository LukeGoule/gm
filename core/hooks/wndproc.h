#pragma once

#ifndef __WNDPROC_H__
#define __WNDPROC_H__

#include <Windows.h>

namespace Hooks 
{
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}

#endif