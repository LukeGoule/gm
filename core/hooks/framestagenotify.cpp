#include "framestagenotify.h"
#include "../hooks.h"
#include "../globals.h"

#include "../../sdk/IBaseClientDLL.h"
#include "../../sdk/IVEngineClient.h"
#include "../../sdk/c_baseentity.h"

namespace Hooks
{
	static void frameUndefined() {}

	static void frameStart() {}

	static void frameNetUpdateStart() {}

	static void frameNetUpdatePostdataupdateStart() {}

	static void frameNetUpdatePostdataupdateEnd() {}

	static void frameNetUpdateEnd() {}

	static void frameRenderStart() 
	{
		if (gm::SDK::Get().EngineClient() &&
			gm::SDK::Get().EngineClient()->IsInGame() &&
			gm::SDK::Get().LocalPlayer() &&
			gm::SDK::Get().LocalPlayer()->get_health() > 0) {

			*gm::SDK::Get().LocalPlayer()->m_bDrawViewmodel() = g_Options.bDrawViewmodel;
		}
	}

	static void frameRenderEnd() {}

	void __fastcall hkFrameStageNotify(
		void* pThis, 
		void* _edx, 
		ClientFrameStage_t stage
	)
	{
		static auto ofunc = client_hook.get_original<decltype(&hkFrameStageNotify)>(Hooks::Indexes::IBaseClientDLL::FrameStageNotify);

		switch (stage)
		{
		case ClientFrameStage_t::FRAME_UNDEFINED: 
			frameUndefined();
			break;
		
		case ClientFrameStage_t::FRAME_START: 
			frameStart();
			break;
		
		case ClientFrameStage_t::FRAME_NET_UPDATE_START: 
			frameNetUpdateStart();
			break;
		
		case ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START: 
			frameNetUpdatePostdataupdateStart();
			break;
		
		case ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END: 
			frameNetUpdatePostdataupdateEnd();
			break;
		
		case ClientFrameStage_t::FRAME_NET_UPDATE_END: 
			frameNetUpdateEnd();
			break;
		
		case ClientFrameStage_t::FRAME_RENDER_START: 
			frameRenderStart(); 
			break;

		case ClientFrameStage_t::FRAME_RENDER_END: 
			frameRenderEnd();
			break;
		}

		ofunc(gm::SDK::Get().ClientDLL(), _edx, stage);
	}
}