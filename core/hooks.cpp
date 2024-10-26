#include "hooks.h"

#include "hooks/runstring.h"
#include "hooks/runstringex.h"
#include "hooks/compilestring.h"
#include "hooks/endscene.h"
#include "hooks/reset.h"
#include "hooks/createmove.h"
#include "hooks/framestagenotify.h"
#include "hooks/lockcursor.h"
#include "hooks/calcview.h"
#include "hooks/wndproc.h"
#include "hooks/viewrender.h"

#include "../menu/GMMenu.h"
#include "../memory/VirtualFunctionHook.h"
#include "../netvars/NetvarDumper.h"

#include "../core/globals.h"

#include "../helpers/utils.h"
#include "../helpers/md5.h"

#include "../sdk.h"
#include "../sdk/CInput.h"
#include "../sdk/IVEngineClient.h"
#include "../sdk/IGameEventManager.h"
#include "../sdk/ISurface.h"
#include "../sdk/IInputSystem.h"
#include "../sdk/CInput.h"
#include "../sdk/C_BaseEntity.h"
#include "../sdk/CViewRender.h"
#include "../sdk/C_GMOD_Player.h"

#include "../detours.h"

#include "../hacks/aimbot.h"
#include "../hacks/beams.h"
#include "../hacks/esp.h"
#include "../hacks/recoil.h"
#include "../hacks/events.h"

#include "../gmod/luaShared.h"
#include "../lua_source/lua.hpp"

extern "C" {
	#include "../lua_source/lstate.h"
}

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"


GameEvents* events;

namespace Hooks {

	using namespace std::literals::chrono_literals;

	VirtualFunctionHook direct3d_hook;
	VirtualFunctionHook client_hook;
	VirtualFunctionHook surface_hook;
	VirtualFunctionHook viewrender_hook;
	VirtualFunctionHook localplayer_hook;

	VirtualFunctionHook lua_clientinterface_hook;
	VirtualFunctionHook lua_serverinterface_hook;
	VirtualFunctionHook lua_menuinterface_hook;

	uintptr_t* TrueCalcView;

	void Init() 
	{

		// Setup ImGui
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(gm::SDK::Get().Window());
		ImGui_ImplDX9_Init(gm::SDK::Get().D3DDevice());

		// Load singleton classes
		ESP::Get().Setup();
		Aimbot::Get().Setup();
		GmMenu::Get().Setup();
		
		// Dump & Load netvar map.
		g_pNetvars = std::make_unique<NetvarDumper>();
		g_pNetvarManager = new NetvarManager;

		direct3d_hook.setup(gm::SDK::Get().D3DDevice());
		client_hook.setup(gm::SDK::Get().ClientDLL());
		surface_hook.setup(gm::SDK::Get().Surface());
		viewrender_hook.setup(gm::SDK::Get().ViewRender());

		direct3d_hook.hook_index(Hooks::Indexes::IDirect3DDevice9::EndScene, hkEndScene);
		direct3d_hook.hook_index(Hooks::Indexes::IDirect3DDevice9::Reset, hkReset);
		client_hook.hook_index(Hooks::Indexes::IBaseClientDLL::CreateMove, hkCreateMove);
		client_hook.hook_index(Hooks::Indexes::IBaseClientDLL::FrameStageNotify, hkFrameStageNotify);
		surface_hook.hook_index(Hooks::Indexes::ISurface::LockCursor, hkLockCursor);
		viewrender_hook.hook_index(Hooks::Indexes::IViewRender::Render, hkViewRender_Render);

		TrueCalcView = (uintptr_t*)Utils::PatternScan(GetModuleHandleA("client.dll"), CALCVIEW_PATTERN);

		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		
		DetourAttach(&(PVOID&)TrueCalcView, Hooks::hkCalcView); 
		Utils::ConsolePrint("Detours hooked CBasePlayer::CalcView\n");

		DetourTransactionCommit();

		events = new GameEvents({ });

		Utils::ConsolePrint(_("[Init] Finished!\n"));

		g_Options.bFullyLoaded = true;
	}

	void Destroy() 
	{
		Sleep(1000); // give the hooks a second.

		direct3d_hook.unhook_all();
		client_hook.unhook_all();
		surface_hook.unhook_all();
		viewrender_hook.unhook_all();

		DestroyLua();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)TrueCalcView, Hooks::hkCalcView);
		DetourTransactionCommit();

		events->~GameEvents();

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		SetWindowLongPtr(
			gm::SDK::Get().Window(), 
			GWLP_WNDPROC, 
			reinterpret_cast<LONG_PTR>(gm::SDK::Get().OriginalWndProc())
		);
	}

	lua::ILuaInterface* pClient = nullptr;
	lua::ILuaInterface* pServer = nullptr;
	lua::ILuaInterface* pMenu = nullptr;

	void CheckLua()
	{
		if (pClient != Hooks::IsLuaClientReady())
		{
			if (Hooks::lua_clientinterface_hook.class_base)
			{
				Hooks::DestroyLuaClient();
				Utils::ConsolePrint("ILuaInterface - Old client (0x%p) unhooked.\n", pClient);
			}

			pClient = Hooks::IsLuaClientReady();

			if (pClient)
			{
				Hooks::InitLuaClient(pClient);
				Utils::ConsolePrint("ILuaInterface - Client hooked - 0x%p\n", pClient);
			}
		}

		if (pServer != Hooks::IsLuaServerReady())
		{
			if (Hooks::lua_serverinterface_hook.class_base)
			{
				Hooks::DestroyLuaServer();
				Utils::ConsolePrint("ILuaInterface - Old server (0x%p) unhooked.\n", pServer);
			}

			pServer = Hooks::IsLuaServerReady();

			if (pServer)
			{
				Hooks::InitLuaServer(pServer);
				Utils::ConsolePrint("ILuaInterface - Server hooked - 0x%p\n", pServer);
			}

		}

		if (pMenu != Hooks::IsLuaMenuReady())
		{
			if (Hooks::lua_menuinterface_hook.class_base)
			{
				Hooks::DestroyLuaMenu();
				Utils::ConsolePrint("ILuaInterface - Old menu (0x%p) unhooked.\n", pMenu);
			}

			pMenu = Hooks::IsLuaMenuReady();

			if (pMenu)
			{
				Hooks::InitLuaMenu(pMenu);
				Utils::ConsolePrint("ILuaInterface - Menu hooked - 0x%p\n", pMenu);
			}
		}
	}

	void InitLuaClient(lua::ILuaInterface* pInterface)
	{
		lua_clientinterface_hook.setup(pInterface);
		lua_clientinterface_hook.hook_index(Hooks::Indexes::ILuaInterface::RunString, hkRunString);
		lua_clientinterface_hook.hook_index(Hooks::Indexes::ILuaInterface::RunStringEx, hkRunStringEx);
		//lua_clientinterface_hook.hook_index(Hooks::Indexes::ILuaInterface::CompileString, hkCompileString);
	}

	void InitLuaServer(lua::ILuaInterface* pInterface)
	{
		lua_serverinterface_hook.setup(pInterface);
		lua_serverinterface_hook.hook_index(Hooks::Indexes::ILuaInterface::RunString, hkRunString);
		lua_serverinterface_hook.hook_index(Hooks::Indexes::ILuaInterface::RunStringEx, hkRunStringEx);
		//lua_serverinterface_hook.hook_index(Hooks::Indexes::ILuaInterface::CompileString, hkCompileString);
	}

	void InitLuaMenu(lua::ILuaInterface* pInterface)
	{
		lua_menuinterface_hook.setup(pInterface);
		lua_menuinterface_hook.hook_index(Hooks::Indexes::ILuaInterface::RunString, hkRunString);
		lua_menuinterface_hook.hook_index(Hooks::Indexes::ILuaInterface::RunStringEx, hkRunStringEx);
		//lua_menuinterface_hook.hook_index(Hooks::Indexes::ILuaInterface::CompileString, hkCompileString);
	}

	void DestroyLuaClient()
	{
		lua_clientinterface_hook.unhook_all();
	}

	void DestroyLuaServer()
	{
		lua_serverinterface_hook.unhook_all();
	}

	void DestroyLuaMenu()
	{
		lua_menuinterface_hook.unhook_all();
	}

	void DestroyLua()
	{
		DestroyLuaClient();
		DestroyLuaServer();
		DestroyLuaMenu();
	}

	lua::ILuaInterface* IsLuaClientReady()
	{
		return gm::SDK::Get().LuaShared()->GetLuaInterface(LUAINTERFACE_CLIENT);
	}

	lua::ILuaInterface* IsLuaServerReady()
	{
		return gm::SDK::Get().LuaShared()->GetLuaInterface(LUAINTERFACE_SERVER);
	}

	lua::ILuaInterface* IsLuaMenuReady()
	{
		return gm::SDK::Get().LuaShared()->GetLuaInterface(LUAINTERFACE_MENU);
	}

	VirtualFunctionHook GetLuaHookmanForInterface(lua::ILuaInterface* pInterface)
	{
		if (!pInterface)
		{
			throw new std::exception{ 
				"pInterface is null" 
			};
		}

		if (pInterface->IsClient())
		{
			return lua_clientinterface_hook;
		}
		else if (pInterface->IsServer())
		{
			return lua_serverinterface_hook;
		}
		else if (pInterface->IsMenu())
		{
			return lua_menuinterface_hook;
		}

		if (!pInterface)
		{
			throw new std::exception{
				"pInterface is not client, server or menu?"
			};
		}
	}



	// interestingly - this actually works, but since beams::CreateBeam doesn't work it don't matter :(
	using fnSetBeamCreationAllowed = void (__fastcall*)(bool state);
	void SetBeamCreationAllowed(bool state) {
		static fnSetBeamCreationAllowed _SetBeamCreationAllowed = (fnSetBeamCreationAllowed)(Utils::PatternScan(GetModuleHandleA(_("client.dll")), _("E8 ? ? ? ? 83 C4 08 E8 ? ? ? ? 8B E5")) + 0x1);

		_SetBeamCreationAllowed(state);
	}
}