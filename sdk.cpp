#include "core/dx.h"

#include "sdk.h"

#include "core/globals.h"

#include "sdk/IAppSystem.h"
#include "sdk/IBaseClientDLL.h"
#include "sdk/IVEngineClient.h"
#include "sdk/IEntityList.h"
#include "sdk/ISurface.h"
#include "sdk/ICvar.h"
#include "sdk/CInput.h"
#include "sdk/IClientMode.h"

#include "sdk/c_baseentity.h"
#include "sdk/IViewRenderBeams.h"
#include "sdk/IGameEventManager.h"
#include "sdk/IInputSystem.h"
#include "sdk/CViewRender.h"
#include "sdk/IVModelInfo.h"

#include "helpers/utils.h"
#include "helpers/obfs.h"

#include "gmod/luaShared.h"
#include "lua_source/lua.hpp"

#include "core/hooks/wndproc.h"

using namespace std::literals::chrono_literals;

void gm::SDK::Initialise()
{
	Utils::ConsolePrint(_("[Init] Waiting for window...\n"));

	while (!(this->m_hWindow = FindWindowA(_("Valve001"), nullptr)))
	{
		std::this_thread::sleep_for(250ms);
		Utils::ConsolePrint(_("[Init] Waiting for window...\n"));
	}

	// override WndProc function to listen in on WINAPI events.
	this->m_poWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(gm::SDK::Get().Window(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Hooks::WndProc)));
	
	// Get refs to DLLs.
	this->m_hClientDll = this->GetDLL(_("client.dll"));
	this->m_hEngineDll = this->GetDLL(_("engine.dll"));
	this->m_hShaderApiDx9Dll = this->GetDLL(_("shaderapidx9.dll"));
	this->m_hLuaSharedDll = this->GetDLL(_("lua_shared.dll"));
	this->m_hVGuiMatSurfaceDll = this->GetDLL(_("vguimatsurface.dll"));
	this->m_hVStdLibDll = this->GetDLL(_("vstdlib.dll"));
	this->m_hInputSystemDll = this->GetDLL(_("inputsystem.dll"));

	// Load interface class instances, using Source interface methods.
	this->m_pClientDLL = interface_get<IBaseClientDLL>(this->m_hClientDll, CLIENT_DLL_VERSION);
	this->m_pEngineClient = interface_get<IVEngineClient>(this->m_hEngineDll, VENGINECLIENT_VERSION);
	this->m_pEntityList = interface_get<IEntityList>(this->m_hClientDll, VCLIENT_ENTITY_LIST_VERSION);
	this->m_pSurface = interface_get<ISurface>(this->m_hVGuiMatSurfaceDll, VGUI_SURFACE_VERSION);
	this->m_pCVar = interface_get<ICvar>(this->m_hVStdLibDll, VENGINE_CVAR_VERSION);
	this->m_pLuaShared = interface_get<lua::CLuaShared>(this->m_hLuaSharedDll, LUA_SHARED_VERSION);
	this->m_pGameEvents = interface_get<IGameEventManager2>(this->m_hEngineDll, GAMEEVENTS_MANAGER_VERSION);
	this->m_pInputSystem = interface_get<IInputSystem>(this->m_hInputSystemDll, INPUT_SYSTEM_VERSION);
	this->m_pModelInfo = interface_get<IVModelInfo>(this->m_hEngineDll, MODEL_INFO_VERSION);

	// Get classes loaded via patterns.
	this->m_pInput = **reinterpret_cast<CInput***>(Utils::PatternScan(this->m_hClientDll, CINPUT_PATTERN) + 0x2);
	this->m_pD3DDevice9 = **reinterpret_cast<LPDIRECT3DDEVICE9**>(Utils::PatternScan(this->m_hShaderApiDx9Dll, D3DDEVICE_PATTERN) + 0x1);
	this->m_pClientMode = *reinterpret_cast<IClientMode**>(Utils::PatternScan(this->m_hClientDll, CLIENTMODE_PATTERN) + 0x2);
	this->m_pBeams = **reinterpret_cast<IViewRenderBeams***>(Utils::PatternScan(this->m_hClientDll, BEAMS_PATTERN) + 0x8);
	this->m_pViewRender = **reinterpret_cast<CViewRender***>(Utils::PatternScan(this->m_hClientDll, VIEWRENDER_PATTERN) + 0x2);
	this->m_pGlobalVars = **reinterpret_cast<CGlobalVarsBase***>(Utils::PatternScan(this->m_hClientDll, GLOBALVARS_PATTERN) + 0x2);

#ifdef _DEBUG
	this->Dump();
#endif
}

void gm::SDK::Dump()
{
	this->PrintInterface<IBaseClientDLL>("IBaseClientDLL", "m_pClientDLL", this->m_pClientDLL);
	this->PrintInterface<IVEngineClient>("IVEngineClient", "m_pEngineClient", this->m_pEngineClient);
	this->PrintInterface<IEntityList>("IEntityList", "m_pEntityList", this->m_pEntityList);
	this->PrintInterface<ISurface>("ISurface", "m_pSurface", this->m_pSurface);
	this->PrintInterface<ICvar>("ICvar", "m_pCVar", this->m_pCVar);
	this->PrintInterface<CInput>("CInput", "m_pInput", this->m_pInput);
	this->PrintInterface<IClientMode>("IClientMode", "m_pClientMode", this->m_pClientMode);
	this->PrintInterface<lua::CLuaShared>("lua::CLuaShared", "m_pLuaShared", this->m_pLuaShared);
	this->PrintInterface<IViewRenderBeams>("IViewRenderBeams", "m_pBeams", this->m_pBeams);
	this->PrintInterface<CViewRender>("CViewRender", "m_pViewRender", this->m_pViewRender);
	this->PrintInterface<IVModelInfo>("IVModelInfo", "m_pModelInfo", this->m_pModelInfo);
	this->PrintInterface<CGlobalVarsBase>("CGlobalVarsBase", "m_pGlobalVars", this->m_pGlobalVars);
}

template <typename T>
void gm::SDK::PrintInterface(std::string Class, std::string Name, T* pInterface)
{
#ifdef _DEBUG
	Utils::ConsolePrint(_("[gm::SDK] %-20s %-20s = 0x%x\n"), Class.c_str(), Name.c_str(), pInterface);
#endif
}

HWND gm::SDK::Window() { return this->m_hWindow; }
LPDIRECT3DDEVICE9 gm::SDK::D3DDevice() { return this->m_pD3DDevice9; }
IBaseClientDLL* gm::SDK::ClientDLL() { return this->m_pClientDLL; }
IVEngineClient* gm::SDK::EngineClient(){ return this->m_pEngineClient; }
IEntityList* gm::SDK::EntityList() { return this->m_pEntityList; }
ISurface* gm::SDK::Surface() { return this->m_pSurface; }
ICvar* gm::SDK::CVar(){ return this->m_pCVar; }
CInput* gm::SDK::Input() { return this->m_pInput; }
IClientMode* gm::SDK::ClientMode() { return this->m_pClientMode; }
lua::CLuaShared* gm::SDK::LuaShared() { return this->m_pLuaShared; }
C_BaseEntity* gm::SDK::LocalPlayer() { return this->m_pLocalPlayer; }
void gm::SDK::UpdateLocalPlayer(C_BaseEntity* pLocalPlayer) { this->m_pLocalPlayer = pLocalPlayer; }
IViewRenderBeams* gm::SDK::Beams() { return this->m_pBeams; }
IGameEventManager2* gm::SDK::GameEvents() { return this->m_pGameEvents; }
IInputSystem* gm::SDK::InputSystem() { return this->m_pInputSystem; }
CViewRender* gm::SDK::ViewRender() { return this->m_pViewRender; }
IVModelInfo* gm::SDK::ModelInfo() { return this->m_pModelInfo;}
CGlobalVarsBase* gm::SDK::GlobalVars() { return this->m_pGlobalVars; }
WNDPROC gm::SDK::OriginalWndProc() { return this->m_poWndProc; }

template<typename T>
T* gm::SDK::GetInterface(CreateInterfaceFn f, const char* szInterfaceVersion)
{
	auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

	if (!result) {
		throw std::runtime_error(std::string(_("[gm::SDK::GetInterface] Failed to GetOffset interface: ")) + szInterfaceVersion);
	}

	return result;
}

HMODULE gm::SDK::GetDLL(const char* szModule)
{
	auto hModule = GetModuleHandleA(szModule);

	if (!hModule) {
		Utils::ConsolePrint(_("[Interfaces] Urgent! Failed to obtain %s!\n"), szModule);
		return nullptr;
	}

	return hModule;
}