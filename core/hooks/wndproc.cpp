#include "wndproc.h"
#include "../hooks.h"

#include "../../menu/GmMenu.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hooks
{
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		const auto getButtonHeld = [uMsg, wParam](bool& bButton, int vKey)
			{
				if (wParam != vKey) return;

				if (uMsg == WM_KEYDOWN)
					bButton = true;
				else if (uMsg == WM_KEYUP)
					bButton = false;
			};

		const auto getButtonToggle = [uMsg, wParam](bool& bButton, int vKey)
			{
				if (wParam != vKey) return;

				if (uMsg == WM_KEYUP) {
					bButton = !bButton;
				}
			};

		getButtonToggle(GmMenu::Get().m_bMenuOpen, VK_F6);

		if (GmMenu::Get().m_bMenuOpen) {
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		}

		return CallWindowProcA(OriginalWNDProc, hWnd, uMsg, wParam, lParam);
	}
}