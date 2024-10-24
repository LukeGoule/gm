#pragma once

#ifndef __FRAMESTAGENOTIFY_H__
#define __FRAMESTAGENOTIFY_H__

enum ClientFrameStage_t;

namespace Hooks
{
	void __fastcall hkFrameStageNotify(
		void* pThis, 
		void* _edx, 
		ClientFrameStage_t stage
	);

}

#endif