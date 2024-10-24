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
			if (pCmd->buttons & IN_JUMP && !(*g_pLocalPlayer->m_fFlags() & FL_ONGROUND))
			{
				pCmd->buttons &= ~IN_JUMP;
			}
		}

		Aimbot::Get().CreateMove(pCmd);
	}

	static void runCreateMove(IBaseClientDLL* pThis, void* _edx, int iSequenceNumber, float fInputSampleFrametime, bool bActive)
	{
		if (!g_pInput) return;

		const bool bPlayerReady = (
			g_pEngineClient && 
			g_pEntityList && 
			g_pEngineClient->IsInGame()
		);

		g_pLocalPlayer = bPlayerReady 
			? reinterpret_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(g_pEngineClient->LocalPlayerID())) 
			: nullptr;

		const bool bInGame = g_pEngineClient->IsInGame() && g_pLocalPlayer;

		CUserCmd* pCmd = g_pInput->GetUserCmd(iSequenceNumber);
		CVerifiedUserCmd* pVerified = g_pInput->GetVerifiedCmd(iSequenceNumber);

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