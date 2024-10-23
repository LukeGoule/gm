#include "sdk.h"

#include <d3d9.h>

#include "globals.h"
#include "IAppSystem.h"
#include "IBaseClientDLL.h"
#include "IVEngineClient.h"
#include "IEntityList.h"
#include "ISurface.h"
#include "ICvar.h"
#include "CInput.h"
#include "IClientMode.h"
#include "gmod/luaShared.h"
#include "lua_source/lua.hpp"
#include "C_BaseEntity.h"
#include "IViewRenderBeams.h"
#include "IGameEventManager.h"
#include "IInputSystem.h"
#include "CViewRender.h"
#include "IVModelInfo.h"

#include "helpers/utils.h"
#include "helpers/obfs.h"

IDirect3DDevice9*	g_pD3DDevice9	= nullptr;
IBaseClientDLL*		g_pClientDLL	= nullptr;
IVEngineClient*		g_pEngineClient = nullptr;
IEntityList*		g_pEntityList	= nullptr;
ISurface*			g_pSurface		= nullptr;
ICvar*				g_pCVar			= nullptr;
CInput*				g_pInput		= nullptr;
IClientMode*		g_pClientMode	= nullptr;
lua::CLuaShared*	g_pLuaShared	= nullptr;
lua_State*			g_pLuaState		= nullptr;
C_BaseEntity*		g_pLocalPlayer	= nullptr;
IViewRenderBeams*	g_pBeams		= nullptr;
IGameEventManager2* g_pGameEvents	= nullptr;
IInputSystem*		g_pInputSystem  = nullptr;
CViewRender*		g_pViewRender	= nullptr;
IVModelInfo*		g_pModelInfo	= nullptr;
CGlobalVarsBase*	g_pGlobalVars = nullptr;

CreateInterfaceFn get_module_factory(HMODULE module)
{
	return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, _("CreateInterface")));
}

template<typename T>
T* get_interface(CreateInterfaceFn f, const char* szInterfaceVersion)
{
	auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

	if (!result) {
		throw std::runtime_error(std::string(_("[get_interface] Failed to GetOffset interface: ")) + szInterfaceVersion);
	}

	return result;
}

void _dump_iface(std::string iname, std::string name, DWORD* iface) {
	Utils::ConsolePrint(_("[Interfaces] %-20s %-20s = 0x%x\n"), iname.c_str(), name.c_str(), (DWORD)iface);
}

HMODULE dllFactory(const char* moduleName) 
{
	auto hModule = GetModuleHandleA(moduleName);

	if (!hModule) {
		Utils::ConsolePrint(_("[Interfaces] Urgent! Failed to obtain %s!\n"), moduleName);
		return nullptr;
	}
	
	return hModule;
}

void SDK::InitInterfaces() 
{
	auto client = dllFactory(_("client.dll"));
	auto engine = dllFactory(_("engine.dll"));
	auto dx9api = dllFactory(_("shaderapidx9.dll"));
	auto lua_shared = dllFactory(_("lua_shared.dll"));
	auto vguimatsurf = dllFactory(_("vguimatsurface.dll"));
	auto vstdlib = dllFactory(_("vstdlib.dll"));
	auto inputsys = dllFactory(_("inputsystem.dll"));

	g_pClientDLL = interface_get<IBaseClientDLL>(client, CLIENT_DLL_VERSION);
	g_pEngineClient = interface_get<IVEngineClient>(engine, VENGINECLIENT_VERSION);
	g_pEntityList = interface_get<IEntityList>(client, VCLIENT_ENTITY_LIST_VERSION);
	g_pSurface = interface_get<ISurface>(vguimatsurf, VGUI_SURFACE_VERSION);
	g_pCVar = interface_get<ICvar>(vstdlib, VENGINE_CVAR_VERSION);
	g_pLuaShared = interface_get<lua::CLuaShared>(lua_shared, LUA_SHARED_VERSION);
	g_pLuaState = SDK::LuaState();
	g_pGameEvents = interface_get<IGameEventManager2>(engine, GAMEEVENTS_MANAGER_VERSION);
	g_pInputSystem = interface_get<IInputSystem>(inputsys, INPUT_SYSTEM_VERSION);
	g_pModelInfo = interface_get<IVModelInfo>(engine, MODEL_INFO_VERSION);

	g_pInput = **reinterpret_cast<CInput***>(Utils::PatternScan(client, CINPUT_PATTERN) + 0x2);
	g_pD3DDevice9 = **reinterpret_cast<IDirect3DDevice9***>(Utils::PatternScan(dx9api, D3DDEVICE_PATTERN) + 0x1);
	g_pClientMode = *reinterpret_cast<IClientMode**>(Utils::PatternScan(client, CLIENTMODE_PATTERN) + 0x2);
	g_pBeams = **reinterpret_cast<IViewRenderBeams***>(Utils::PatternScan(client, BEAMS_PATTERN) + 0x8);
	g_pViewRender = **reinterpret_cast<CViewRender***>(Utils::PatternScan(client, VIEWRENDER_PATTERN) + 0x2);
	g_pGlobalVars = **reinterpret_cast<CGlobalVarsBase***>(Utils::PatternScan(client, GLOBALVARS_PATTERN) + 0x2);
}

void SDK::DumpInterfaces() {
	DUMP_IFACE(IDirect3DDevice9, g_pD3DDevice9);
	DUMP_IFACE(IBaseClientDLL, g_pClientDLL);
	DUMP_IFACE(IVEngineClient, g_pEngineClient);
	DUMP_IFACE(IEntityList, g_pEntityList);
	DUMP_IFACE(ISurface, g_pSurface);
	DUMP_IFACE(ICvar, g_pCVar);
	DUMP_IFACE(CInput, g_pInput);
	DUMP_IFACE(IClientMode, g_pClientMode);
	DUMP_IFACE(lua::CLuaShared, g_pLuaShared);
	DUMP_IFACE(lua_State, g_pLuaState);
	DUMP_IFACE(IViewRenderBeams, g_pBeams);
	DUMP_IFACE(CViewRender, g_pViewRender);
	DUMP_IFACE(IVModelInfo, g_pModelInfo);
	DUMP_IFACE(CGlobalVarsBase, g_pGlobalVars);
}

lua_State* SDK::LuaState() {
	lua::ILuaBase* lua = nullptr;

	// if we're not in the game, the lua interface for client will not exist, i think lmfao
	if (g_pEngineClient->IsInGame()) {
		lua = g_pLuaShared->GetLuaInterface(LUAINTERFACE_CLIENT);
	}
	else {
		lua = g_pLuaShared->GetLuaInterface(LUAINTERFACE_MENU);
	}

	if (!lua) {
		return nullptr;
	}

	return lua->GetState();
}