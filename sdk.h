#pragma once

#include <iostream>
#include <iostream>
#include <intrin.h>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <TlHelp32.h>

#include "netvars/NetvarManager.h"
#include "lua_source/lua.hpp"

#include "helpers/utils.h"
#include "helpers/obfs.h"
#include "helpers/singleton.h"

#include "sdk/IAppSystem.h"

#define DUMP_IFACE(classname, i)	_dump_iface(#classname, #i, (DWORD*)i)

#define CLIENT_DLL_VERSION			"VClient017"
#define VENGINECLIENT_VERSION		"VEngineClient"
#define VCLIENT_ENTITY_LIST_VERSION "VClientEntityList"
#define VGUI_SURFACE_VERSION		"VGUI_Surface030"
#define VENGINE_CVAR_VERSION		"VEngineCvar"
#define LUA_SHARED_VERSION			"LUASHARED"
#define GAMEEVENTS_MANAGER_VERSION  "GAMEEVENTSMANAGER002"
#define INPUT_SYSTEM_VERSION		"InputSystemVersion001"
#define MODEL_INFO_VERSION			"VModelInfoClient006"

#define CINPUT_PATTERN				"8B 0D ? ? ? ? 8B 01 FF 60 44"
#define D3DDEVICE_PATTERN			"A1 ? ? ? ? 50 8B 08 FF 51 0C"
#define CLIENTMODE_PATTERN			"8B 0D ? ? ? ? 8B 01 5D"
#define BEAMS_PATTERN				"89 86 ? ? ? ? 8B 0D ? ? ? ? 8B 01 FF 10" // https://www.unknowncheats.me/forum/counterstrike-global-offensive/240066-cs-beams-demystified.html

// SigMaker failed for me here so this was manually calculated. works tho :)
/*
pviewrender = 51 79 06 58
	  58 06 79 51                                              58 06 79 51
8B 0D ?? ?? ?? ?? 50 FF 57 7C 8B 4D F8 53 8B 01 FF 50 28 8B 0D ?? ?? ?? ?? 6A 00 8B 01 FF 50 7C
*/
#define VIEWRENDER_PATTERN			"8B 0D ?? ?? ?? ?? 50 FF 57 7C 8B 4D F8 53 8B 01 FF 50 28 8B 0D ?? ?? ?? ?? 6A 00 8B 01 FF 50 7C"
#define GLOBALVARS_PATTERN			"8B 0D ? ? ? ? 83 79 14 01 7E 12 6A 01 6A 01 E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 08"
#define CALCVIEW_PATTERN			"55 8B EC 53 56 8B F1 57 8B 8E 10 25 00 00 83 F9 FF 0F 84 ? ? ? ? 8B 15 ? ? ? ? 8B C1 25 FF 3F 00 00 C1 E9 0E 03 C0 39 4C C2 08 0F 85 ? ? ? ? 8B 4C C2 04 85 C9 0F 84 ? ? ? ? 8B 01 FF 90 5C 01 00 00 8B D8 85 DB 0F 84 ? ? ? ? 8B CE E8 ? ? ? ? D9 86 D4 25 00 00"

namespace SDK 
{
	void InitInterfaces();
	void DumpInterfaces();
}

class IBaseClientDLL;
class IVEngineClient;
class IEntityList;
class ISurface;
class ICvar;
class CInput;
class IClientMode;
class C_BaseEntity;
class IViewRenderBeams;
class IGameEventManager2;
class IInputSystem;
class CViewRender;
class IVModelInfo;
class CGlobalVarsBase;

struct IDirect3DDevice9;
typedef struct IDirect3DDevice9* LPDIRECT3DDEVICE9, * PDIRECT3DDEVICE9;

namespace lua
{
	class CLuaShared;
}

namespace gm
{
	class SDK : public Singleton<SDK>
	{
	private:

		HWND m_hWindow = nullptr;
		WNDPROC m_poWndProc = nullptr;

		HMODULE m_hClientDll = nullptr;
		HMODULE m_hEngineDll = nullptr;
		HMODULE m_hShaderApiDx9Dll = nullptr;
		HMODULE m_hLuaSharedDll = nullptr;
		HMODULE m_hVGuiMatSurfaceDll = nullptr;
		HMODULE m_hVStdLibDll = nullptr;
		HMODULE m_hInputSystemDll = nullptr;

		LPDIRECT3DDEVICE9 m_pD3DDevice9 = nullptr;
		IBaseClientDLL* m_pClientDLL = nullptr;
		IVEngineClient* m_pEngineClient = nullptr;
		IEntityList* m_pEntityList = nullptr;
		ISurface* m_pSurface = nullptr;
		ICvar* m_pCVar = nullptr;
		CInput* m_pInput = nullptr;
		IClientMode* m_pClientMode = nullptr;
		lua::CLuaShared* m_pLuaShared = nullptr;
		C_BaseEntity* m_pLocalPlayer = nullptr;
		IViewRenderBeams* m_pBeams = nullptr;
		IGameEventManager2* m_pGameEvents = nullptr;
		IInputSystem* m_pInputSystem = nullptr;
		CViewRender* m_pViewRender = nullptr;
		IVModelInfo* m_pModelInfo = nullptr;
		CGlobalVarsBase* m_pGlobalVars = nullptr;

	public:

		void Initialise();
		void Dump();

	private:

		template <typename T>
		void PrintInterface(std::string Class, std::string Name, T* pInterface);

	public:

		HWND Window();
		LPDIRECT3DDEVICE9 D3DDevice();
		IBaseClientDLL* ClientDLL();
		IVEngineClient* EngineClient();
		IEntityList* EntityList();
		ISurface* Surface();
		ICvar* CVar();
		CInput* Input();
		IClientMode* ClientMode();
		lua::CLuaShared* LuaShared();
		C_BaseEntity* LocalPlayer();
		void UpdateLocalPlayer(C_BaseEntity* pLocalPlayer);
		IViewRenderBeams* Beams();
		IGameEventManager2* GameEvents();
		IInputSystem* InputSystem();
		CViewRender* ViewRender();
		IVModelInfo* ModelInfo();
		CGlobalVarsBase* GlobalVars();
		WNDPROC OriginalWndProc();

	private:
		
		template<typename T>
		T* GetInterface(CreateInterfaceFn f, const char* szInterfaceVersion);

		HMODULE GetDLL(const char* szModule);
	};
}