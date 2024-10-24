#pragma once

#ifndef __CREATEMOVE_H__
#define __CREATEMOVE_H__

class IBaseClientDLL; // sdk/ibaseclientdll.h

namespace Hooks
{
	void __fastcall hkCreateMove(
		IBaseClientDLL* pThis,
		void* _edx,
		int iSequenceNumber,
		float fInputSampleTime,
		bool bActive
	);
}

#endif