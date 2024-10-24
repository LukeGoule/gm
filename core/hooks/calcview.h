#pragma once

#ifndef __CALCVIEW_H__
#define __CALCVIEW_H__

#include "../../helpers/vector.h"
#include "../../helpers/qangle.h"

namespace Hooks
{
	int  __fastcall hkCalcView(
		void* pThis, 
		void* _edx, 
		Vector& vecEyeOrigin, 
		QAngle& qanEyeAngles, 
		float& fzNear, 
		float& fzFar, 
		float& fFov
	);
}

#endif

