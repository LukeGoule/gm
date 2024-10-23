#include "IVEngineClient.h"
#include "gmod/luaShared.h"
#include "globals.h"

std::string IVEngineClient::GetCurrentMap() {
	if (!g_pLuaShared) return {};
	if (!this->IsInGame()) return {};

	auto LUA = g_pLuaShared->GetLuaInterface(LUAINTERFACE_CLIENT);
	if (!LUA) return {};

	// i was on the verge of killing someone trying to get this to work
	// fuck the stack bruh
	LUA->PushSpecial((int)lua::special::glob);	// 1
	LUA->GetField(-1, _("game"));				// 2
	LUA->GetField(-1, _("GetMap"));				// 3
	LUA->Call(0, 1);							// 1
	auto ret = LUA->GetString();				// 2
	LUA->Pop(3);								// 0

	return std::string(ret);
}