#include "calcview.h"
#include "../hooks.h"

#include "../globals.h"
#include "../../sdk/c_baseentity.h"

#include "../../hacks/aimbot.h"

namespace Hooks
{
	int __fastcall hkCalcView(void* pThis, void* _edx, Vector& vecEyeOrigin, QAngle& qEyeAngles, float& fzNear, float& fzFar, float& fFov)
	{
		auto oCalcView = (decltype(&hkCalcView))Hooks::TrueCalcView;

		Vector ovPunchAngle = *gm::SDK::Get().LocalPlayer()->m_vecPunchAngle();

		if (!Aimbot::Get().bUseFASNoSpread)
		{
			*gm::SDK::Get().LocalPlayer()->m_vecPunchAngle() = Vector(0, 0, 0);
		}

		auto ret = oCalcView(pThis, _edx, vecEyeOrigin, qEyeAngles, fzNear, fzFar, fFov);

		if (!Aimbot::Get().bUseFASNoSpread) 
		{
			*gm::SDK::Get().LocalPlayer()->m_vecPunchAngle() = ovPunchAngle;
		}

		return ret;
	}
}