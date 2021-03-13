#pragma once

#include "singleton.h"
#include "sdk.h"
#include "IBaseClientDLL.h"

class Recoil : public Singleton<Recoil> {
public:

	void FrameStageNotify_Begin(ClientFrameStage_t Stage);
	void FrameStageNotify_End(ClientFrameStage_t Stage);
};