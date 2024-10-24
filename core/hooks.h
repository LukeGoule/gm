#pragma once

#include "../memory/VirtualFunctionHook.h"

#undef PlaySound

class Vector;
class QAngle;

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
			constexpr auto RunStringEx = 112;
			constexpr auto CompileString = 117;
		}
	}

	void Init();
	void Destroy();

	extern VirtualFunctionHook direct3d_hook;
	extern VirtualFunctionHook client_hook;
	extern VirtualFunctionHook surface_hook;
	extern VirtualFunctionHook viewrender_hook;
	extern VirtualFunctionHook localplayer_hook;
	extern VirtualFunctionHook luainterface_hook;

	extern QAngle qCurrentAngle;
	extern HWND	GMODWindow;
	extern WNDPROC OriginalWNDProc;
	extern uintptr_t* TrueCalcView;
}