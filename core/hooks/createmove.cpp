#include "createmove.h"

#include "../hooks.h"
#include "../globals.h"

#include "../../helpers/qangle.h"
#include "../../sdk/CInput.h"
#include "../../sdk/IVEngineClient.h"
#include "../../sdk/IEntityList.h"
#include "../../sdk/c_baseentity.h"

#include "../../hacks/aimbot.h"

namespace Hooks
{
	QAngle qCurrentAngle;

	static void runCheats(bool bPlayerReady, bool bInGame, CUserCmd* pCmd)
	{
		if (!bInGame) return;

		if (g_Options.bBhop)
		{
			if (pCmd->buttons & IN_JUMP && !(*gm::SDK::Get().LocalPlayer()->m_fFlags() & FL_ONGROUND))
			{
				pCmd->buttons &= ~IN_JUMP;
			}
		}

		Aimbot::Get().CreateMove(pCmd);
	}

	static void runCreateMove(IBaseClientDLL* pThis, void* _edx, int iSequenceNumber, float fInputSampleFrametime, bool bActive)
	{
		if (!gm::SDK::Get().Input()) return;

		const bool bPlayerReady = (
			gm::SDK::Get().EngineClient() &&
			gm::SDK::Get().EntityList() &&
			gm::SDK::Get().EngineClient()->IsInGame()
		);

		gm::SDK::Get().UpdateLocalPlayer(bPlayerReady 
			? reinterpret_cast<C_BaseEntity*>(gm::SDK::Get().EntityList()->GetClientEntity(
				gm::SDK::Get().EngineClient()->LocalPlayerID()
			))
			: nullptr
		);

		const bool bInGame = gm::SDK::Get().EngineClient()->IsInGame() && gm::SDK::Get().LocalPlayer();

		CUserCmd* pCmd = gm::SDK::Get().Input()->GetUserCmd(iSequenceNumber);
		CVerifiedUserCmd* pVerified = gm::SDK::Get().Input()->GetVerifiedCmd(iSequenceNumber);

		runCheats(bPlayerReady, bInGame, pCmd);

		qCurrentAngle = pCmd->view_angles;

		pVerified->m_cmd = *pCmd;
		pVerified->m_crc = pCmd->get_checksum();
	}
	
	void __fastcall hkCreateMove(IBaseClientDLL* pThis, void* _edx, int iSequenceNumber, float fInputSampleFrametime, bool bActive)
	{
		static auto oCreateMove = client_hook.get_original<decltype(&hkCreateMove)>(Hooks::Indexes::IBaseClientDLL::CreateMove);
		oCreateMove(pThis, _edx, iSequenceNumber, fInputSampleFrametime, bActive);
		runCreateMove(pThis, _edx, iSequenceNumber, fInputSampleFrametime, bActive);
	}
}