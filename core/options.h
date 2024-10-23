#pragma once

#include "../imgui/imgui.h"

class options {
public:
	bool bBhop					= true;					// Menu: Implemented
	bool bNoKick				= true;					// Menu: Implemented
	bool bDrawViewmodel			= true;					// Menu: Implemented
	bool bOutputLogToConsole	= true;					// Menu: Implemented
	bool bFullyLoaded			= false;				// Menu: N/A
	bool bDrawWaterMark			= false;				// Menu: Implemented
	ImVec4 vecMenuColorScheme = ImVec4(34.f/255, 87.f/255.f, 140.f/255.f, 1.f);	// Menu: Implemented

	bool bESPMasterEnabled		= true;					// Menu: Implemented
	bool bESPDrawBoxes			= true;					// Menu: Implemented
	bool bESPDrawClassName		= true;					// Menu: Implemented
	bool bESPDrawPlayerNames	= true;					// Menu: Implemented
	bool bESPShowNPCs			= true;					// Menu: Implemented
	bool bESPShowWeapons		= true;					// Menu: Implemented
	bool bESPShowPlayers		= true;					// Menu: Implemented
	int iESPMaxDrawDist = 10000;						// Menu: Implemented

	bool bAimMasterEnabled		= true;					// Menu: Implemented
	bool bAimSilent				= true;					// Menu: Implemented
	bool bAimAutofire			= false;				// Menu: Implemented
	bool bAimTargetPlayers		= true;					// Menu: Implemented
	bool bAimTargetNPCs			= true;					// Menu: Implemented
	bool bAimRemoveSpread		= true;					// Menu: Implemented
	int iTargetBoneID			= 8;
	void* pCurrentTarget		= 0;					// Menu: N/A
};

extern options o;