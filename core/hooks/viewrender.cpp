#include "viewrender.h"
#include "../hooks.h"

#include "../globals.h"
#include "../../sdk/CViewRender.h"
#include "../../sdk/IVEngineClient.h"
#include "../../sdk/c_baseentity.h"

namespace Hooks
{
	void __fastcall hkViewRender_Render(CViewRender* _this, void* _edx, vrect_t* rect) 
	{
		static auto ofunc = viewrender_hook.get_original<decltype(&hkViewRender_Render)>(Hooks::Indexes::IViewRender::Render);

		if (!(gm::SDK::Get().EngineClient() && gm::SDK::Get().EngineClient()->IsInGame() && gm::SDK::Get().LocalPlayer() && gm::SDK::Get().LocalPlayer()->is_alive()))
		{
			ofunc(_this, _edx, rect);
			return;
		}

		ofunc(_this, _edx, rect);
	}
}