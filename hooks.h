#pragma once

#include <d3d9.h>

#include "VirtualFunctionHook.h"

namespace gmod_hook_indexes
{
	constexpr auto EmitSound1				= 5;
	constexpr auto EmitSound2				= 6;
	constexpr auto EndScene					= 42;
	constexpr auto Reset					= 16;
	constexpr auto PaintTraverse			= 41;
	constexpr auto CreateMove				= 21;
	constexpr auto PlaySound				= 82;
	constexpr auto FrameStageNotify			= 35;
	constexpr auto DrawModelExecute			= 21;
	constexpr auto DoPostScreenSpaceEffects = 44;
	constexpr auto SvCheatsGetBool			= 13;
	constexpr auto OverrideView				= 18;
	constexpr auto LockCursor				= 62;
	constexpr auto SceneEnd					= 9;
	constexpr auto SendDatagram				= 46;
	constexpr auto WriteUsercmdDeltaToBuffer = 24;
	constexpr auto ViewRender_Render		= 5;
	constexpr auto LocalPlayer_CalcView		= 282;
	constexpr auto ILuaInterface_RunString  = 92;
}

class Vector;
class QAngle;
class IBaseClientDLL;
class CViewRender;
struct vrect_t;
enum ClientFrameStage_t;

namespace lua {
	class ILuaInterface;
}

class C_PointCamera { };

namespace Hooks {
	void Init();
	void Destroy();

	bool __fastcall hkRunString(lua::ILuaInterface* thisptr, int edx, const char* filename, const char* path, const char* stringToRun, bool run, bool showErrors);
	void __fastcall hkCreateMove(IBaseClientDLL* thiscall, void* _EDX, int sequence_number, float input_sample_frametime, bool active);
	void __fastcall hkFrameStageNotify(void* _this, int edx, ClientFrameStage_t stage);
	int  __fastcall hkCalcView(DWORD* thisptr, int edx, Vector& eyeOrigin, QAngle& eyeAngles, float& zNear, float& zFar, float& fov);
	void __fastcall hkViewRender_Render(CViewRender* _this, int edx, vrect_t* rect);
	long __stdcall hkEndScene(IDirect3DDevice9* pDevice);
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void __fastcall hkLockCursor(void* _this);

	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	extern QAngle qCurrentAngle;
	extern VirtualFunctionHook direct3d_hook;
	extern VirtualFunctionHook client_hook;
	extern VirtualFunctionHook surface_hook;
	extern VirtualFunctionHook viewrender_hook;
	extern VirtualFunctionHook localplayer_hook;
	extern HWND	GMODWindow;
	extern WNDPROC OriginalWNDProc;
	extern uintptr_t* TrueCalcView;
}