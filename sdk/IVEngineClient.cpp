#include "IVEngineClient.h"

#include "../gmod/luaShared.h"
#include "../core/globals.h"

#include "../../sdk.h"

std::string IVEngineClient::GetCurrentMap() {
	if (!gm::SDK::Get().LuaShared()) return {};
	if (!this->IsInGame()) return {};

	auto LUA = gm::SDK::Get().LuaShared()->GetLuaInterface(LUAINTERFACE_CLIENT);
	if (!LUA) return {};

	LUA->PushSpecial((int)lua::special::glob);	// 1
	LUA->GetField(-1, _("game"));				// 2
	LUA->GetField(-1, _("GetMap"));				// 3
	LUA->Call(0, 1);							// 1
	auto ret = LUA->GetString();				// 2
	LUA->Pop(3);								// 0

	return std::string(ret);
}