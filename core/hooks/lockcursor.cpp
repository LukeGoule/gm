#include "lockcursor.h"
#include "../hooks.h"

#include "../../sdk.h"

#include "../globals.h"

#include "../../menu/GmMenu.h"
#include "../../sdk/IInputSystem.h"
#include "../../sdk/IVEngineClient.h"
#include "../../sdk/ISurface.h"

namespace Hooks
{
	void __fastcall hkLockCursor(void* _this)
	{
		static auto ofunc = surface_hook.get_original<decltype(&hkLockCursor)>(Hooks::Indexes::ISurface::LockCursor);
		
		gm::SDK::Get().InputSystem()->EnableInput(!GmMenu::Get().m_bMenuOpen);

		if (gm::SDK::Get().EngineClient()->IsInGame() &&
			GmMenu::Get().m_bMenuOpen) {
			gm::SDK::Get().Surface()->unlock_cursor();
		}
		else
		{
			ofunc(gm::SDK::Get().Surface());
		}
	}
}