#pragma once

#include "../helpers/singleton.h"
#include "../sdk.h"
#include "../sdk/IBaseClientDLL.h"

class Recoil : public Singleton<Recoil> {
public:

	void FrameStageNotify_Begin(ClientFrameStage_t Stage);
	void FrameStageNotify_End(ClientFrameStage_t Stage);
};