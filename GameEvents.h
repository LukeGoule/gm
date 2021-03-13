#pragma once

#include "IGameEventManager.h"

#include <vector>

class GameEvents : public IGameEventListener2
{
public:
	GameEvents(std::vector<const char*> events);
	~GameEvents();
	void FireGameEvent(IGameEvent* event) override;
	int GetEventDebugID() override;

};