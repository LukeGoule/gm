#pragma once

#include "../memory/VirtualFunctionHook.h"

#undef PlaySound

class Vector;
class QAngle;

namespace lua
{
	class ILuaInterface;
}

namespace Hooks 
{
	namespace Indexes
	{
		constexpr auto EmitSound1 = 5;
		constexpr auto EmitSound2 = 6;
		constexpr auto PaintTraverse = 41;
		constexpr auto PlaySound = 82;
		constexpr auto DrawModelExecute = 21;
		constexpr auto DoPostScreenSpaceEffects = 44;
		constexpr auto SvCheatsGetBool = 13;
		constexpr auto OverrideView = 18;
		constexpr auto SceneEnd = 9;
		constexpr auto SendDatagram = 46;
		constexpr auto WriteUsercmdDeltaToBuffer = 24;

		namespace IDirect3DDevice9
		{
			constexpr auto EndScene = 42;
			constexpr auto Reset = 16;

		}
		
		namespace IBaseClientDLL
		{
			constexpr auto CreateMove = 21;
			constexpr auto FrameStageNotify = 35;
		}
		
		namespace ISurface
		{
			constexpr auto LockCursor = 62;
		}
		
		namespace IViewRender
		{
			constexpr auto Render = 5;
		}

		namespace CLocalPlayer
		{
			constexpr auto CalcView = 282;
		}

		namespace ILuaInterface 
		{
			constexpr auto RunString = 92;
			constexpr auto RunStringEx = 111;
			constexpr auto CompileString = 117;
		}
	}

	void Init();
	void Destroy();

	// Hook / unhook lua.
	void CheckLua();

	// Special methods to hook & unhook lua states when they become available / stop being available.
	void InitLuaClient(lua::ILuaInterface* pInterface);
	void InitLuaServer(lua::ILuaInterface* pInterface);
	void InitLuaMenu(lua::ILuaInterface* pInterface);

	void DestroyLuaClient();
	void DestroyLuaServer();
	void DestroyLuaMenu();

	void DestroyLua();

	// Is the client state interface ready?
	lua::ILuaInterface* IsLuaClientReady();
	
	// Is the server state interface ready?
	lua::ILuaInterface* IsLuaServerReady();

	// Is the menu state interface ready?
	lua::ILuaInterface* IsLuaMenuReady();

	// Get the appropriate hook manager for the given interface.
	VirtualFunctionHook GetLuaHookmanForInterface(lua::ILuaInterface* pInterface);

	extern VirtualFunctionHook direct3d_hook;
	extern VirtualFunctionHook client_hook;
	extern VirtualFunctionHook surface_hook;
	extern VirtualFunctionHook viewrender_hook;
	extern VirtualFunctionHook localplayer_hook;

	extern VirtualFunctionHook lua_clientinterface_hook;
	extern VirtualFunctionHook lua_serverinterface_hook;
	extern VirtualFunctionHook lua_menuinterface_hook;

	extern QAngle qCurrentAngle;
	extern HWND	GMODWindow;
	extern WNDPROC OriginalWNDProc;
	extern uintptr_t* TrueCalcView;
}