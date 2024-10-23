// game_newmap

#include "events.h"
#include "beams.h"


#include "../helpers/math.h"
#include "../helpers/obfs.h"

#include "../sdk.h"
#include "../globals.h"
#include "../IGameEventManager.h"
#include "../IVEngineClient.h"
#include "../C_BaseEntity.h"

GameEvents::GameEvents(std::vector<const char*> events) {
	for (auto& it : events) {
		g_pGameEvents->AddListener(this, it, false);
		Utils::ConsolePrint(_("[GameEvents] Registered: %s\n"), it);
	}
}

GameEvents::~GameEvents() {
	g_pGameEvents->RemoveListener(this);
}

void GameEvents::FireGameEvent(IGameEvent* event) {

	Utils::ConsolePrint(_("%s\n"), event->GetName());

	if (strcmp(event->GetName(), _("round_start")) == 0) { // gmod: doesn't work
		// ...
	}

	if (strcmp(event->GetName(), _("game_newmap")) == 0) { // gmod: doesn't work
		// ...
	}

	if (strcmp(event->GetName(), _("bullet_impact")) == 0) { // gmod: doesn't work
		// ...
	}

	if (strcmp(event->GetName(), _("player_death")) == 0) {
		// ...
	}
}

int GameEvents::GetEventDebugID() {
	return EVENT_DEBUG_ID_INIT;
}
