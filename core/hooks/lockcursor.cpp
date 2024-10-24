#include "lockcursor.h"
#include "../hooks.h"

#include "../globals.h"

#include "../../menu/GmMenu.h"
#include "../../sdk/IInputSystem.h"
#include "../../sdk/IVEngineClient.h"
#include "../../sdk/ISurface.h"

namespace Hooks
{
	void runLockCursorMod()
	{
		if (GmMenu::Get().m_bRunning)
			g_pInputSystem->EnableInput(!GmMenu::Get().m_bMenuOpen);
		else
			g_pInputSystem->EnableInput(true);

		if (g_pEngineClient->IsInGame() &&
			GmMenu::Get().m_bMenuOpen) {
			g_pSurface->unlock_cursor();
		}
	}

	void __fastcall hkLockCursor(void* _this)
	{
		static auto ofunc = surface_hook.get_original<decltype(&hkLockCursor)>(Hooks::Indexes::ISurface::LockCursor);
		runLockCursorMod();
		ofunc(g_pSurface);
	}
}