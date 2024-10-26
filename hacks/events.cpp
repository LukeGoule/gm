#include "events.h"
#include "beams.h"

#include "../helpers/math.h"
#include "../helpers/obfs.h"

#include "../sdk.h"
#include "../sdk/IGameEventManager.h"
#include "../sdk/c_baseentity.h"
#include "../sdk/IVEngineClient.h"

#include "../core/globals.h"

GameEvents::GameEvents(std::vector<const char*> events) 
{
	for (auto& it : events) {
		gm::SDK::Get().GameEvents()->AddListener(this, it, false);
		Utils::ConsolePrint(_("[GameEvents] Registered: %s\n"), it);
	}
}

GameEvents::~GameEvents() 
{
	gm::SDK::Get().GameEvents()->RemoveListener(this);
}

void GameEvents::FireGameEvent(IGameEvent* event) 
{

	Utils::ConsolePrint(_("%s\n"), event->GetName());

	if (strcmp(event->GetName(), _("round_start")) == 0) 
	{ 
		// ...
	}

	if (strcmp(event->GetName(), _("game_newmap")) == 0) 
	{
		// ...
	}

	if (strcmp(event->GetName(), _("bullet_impact")) == 0) 
	{
		// ...
	}

	if (strcmp(event->GetName(), _("player_death")) == 0) 
	{
		// ...
	}
}

int GameEvents::GetEventDebugID() 
{
	return EVENT_DEBUG_ID_INIT;
}
