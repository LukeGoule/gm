#include "hooks.h"

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

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

GameEvents* events;
bool obInGame = false;

namespace Hooks {

	using namespace std::literals::chrono_literals;

	VirtualFunctionHook direct3d_hook;
	VirtualFunctionHook client_hook;
	VirtualFunctionHook surface_hook;
	VirtualFunctionHook viewrender_hook;
	VirtualFunctionHook localplayer_hook;
	VirtualFunctionHook luainterface_hook; // luainterface client.

	HWND GMODWindow;
	WNDPROC OriginalWNDProc;

	uintptr_t* TrueCalcView;

	void Init() {
		Utils::ConsolePrint(_("[Init] Looking for window...\n"));
		while (!(GMODWindow = FindWindowA(_("Valve001"), nullptr)))
		{
			std::this_thread::sleep_for(250ms);
			Utils::ConsolePrint(_("[Init] Looking for window...\n"));
		}

		Utils::ConsolePrint(_("[Init] Found window.\n"));

		if (GMODWindow) {
			OriginalWNDProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(GMODWindow, GWLP_WNDPROC,
				reinterpret_cast<LONG_PTR>(WndProc)));
		}

		while (!g_pLuaShared->GetLuaInterface(LUAINTERFACE_CLIENT));

		// Setup ImGui
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(GMODWindow);
		ImGui_ImplDX9_Init(g_pD3DDevice9);

		// Load singleton classes
		ESP::Get().Setup();
		Aimbot::Get().Setup();
		GmMenu::Get().Setup();
		
		// Dump & Load netvar map.
		g_pNetvars = std::make_unique<NetvarDumper>();
		g_pNetvarManager = new NetvarManager;

		auto l_cl = g_pLuaShared->GetLuaInterface(LUAINTERFACE_CLIENT);

		direct3d_hook.setup(g_pD3DDevice9);
		client_hook.setup(g_pClientDLL);
		surface_hook.setup(g_pSurface);
		viewrender_hook.setup(g_pViewRender);
		luainterface_hook.setup(l_cl);

		direct3d_hook.hook_index(gmod_hook_indexes::EndScene, hkEndScene);
		direct3d_hook.hook_index(gmod_hook_indexes::Reset, hkReset);
		client_hook.hook_index(gmod_hook_indexes::CreateMove, hkCreateMove);
		client_hook.hook_index(gmod_hook_indexes::FrameStageNotify, hkFrameStageNotify);
		surface_hook.hook_index(gmod_hook_indexes::LockCursor, hkLockCursor);
		viewrender_hook.hook_index(gmod_hook_indexes::ViewRender_Render, hkViewRender_Render);
		luainterface_hook.hook_index(gmod_hook_indexes::ILuaInterface_RunString, hkRunString);

		TrueCalcView = (uintptr_t*)Utils::PatternScan(GetModuleHandleA("client.dll"), CALCVIEW_PATTERN);

		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		
		DetourAttach(&(PVOID&)TrueCalcView, Hooks::hkCalcView); Utils::ConsolePrint("Detours hooked CBasePlayer::CalcView\n");

		DetourTransactionCommit();

		events = new GameEvents({ });

		Utils::ConsolePrint(_("[Init] Finished!\n"));

		o.bFullyLoaded = true;
		obInGame = g_pEngineClient->IsInGame(); // we should always be injecting when the mainmenu has started up, but for debug purposes this is acceptable.
	}

	void Destroy() {
		Sleep(100); // give the hooks a second.
		direct3d_hook		.unhook_all();
		client_hook			.unhook_all();
		surface_hook		.unhook_all();
		viewrender_hook		.unhook_all();
		luainterface_hook	.unhook_all();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)TrueCalcView, Hooks::hkCalcView);
		DetourTransactionCommit();

		events->~GameEvents();

		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		SetWindowLongPtr(GMODWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(OriginalWNDProc));
	}

	bool __fastcall hkRunString(lua::ILuaInterface* thisptr, int edx, const char* filename, const char* path, const char* stringToRun, bool run, bool showErrors) {
		static decltype(&hkRunString) oRunString = luainterface_hook.get_original<decltype(&hkRunString)>(gmod_hook_indexes::ILuaInterface_RunString);
		{
			Utils::ConsolePrint("str run: %s\n", stringToRun);
		}
		return oRunString(thisptr, edx, filename, path, stringToRun, run, showErrors);
	}

	// interestingly - this actually works, but since beams::CreateBeam doesn't work it don't matter :(
	using fnSetBeamCreationAllowed = void (__fastcall*)(bool state);
	void SetBeamCreationAllowed(bool state) {
		static fnSetBeamCreationAllowed _SetBeamCreationAllowed = (fnSetBeamCreationAllowed)(Utils::PatternScan(GetModuleHandleA(_("client.dll")), _("E8 ? ? ? ? 83 C4 08 E8 ? ? ? ? 8B E5")) + 0x1);
		
		_SetBeamCreationAllowed(state);
	}

	QAngle qCurrentAngle;
	void __fastcall hkCreateMove(IBaseClientDLL* thisptr, void* _EDX, int sequence_number, float input_sample_frametime, bool active) {
		static auto oCreateMove = client_hook.get_original<decltype(&hkCreateMove)>(gmod_hook_indexes::CreateMove);

		oCreateMove(thisptr, _EDX, sequence_number, input_sample_frametime, active);

		if (!g_pInput) {
			Utils::ConsolePrint(_("SDK::Input() == 0!!\n"));
			return;
		}
		
		if (g_pEngineClient && g_pEntityList && g_pEngineClient->IsInGame()) {
			g_pLocalPlayer = reinterpret_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(g_pEngineClient->LocalPlayerID()));
		}
		else {
			g_pLocalPlayer = nullptr;
		}

		CUserCmd* pCmd					= g_pInput->GetUserCmd(sequence_number);
		CVerifiedUserCmd* pVerified		= g_pInput->GetVerifiedCmd(sequence_number);

		if (g_pEngineClient->IsInGame() && g_pLocalPlayer) {
			if (o.bBhop) {
				if (pCmd->buttons & IN_JUMP && !(*g_pLocalPlayer->m_fFlags() & FL_ONGROUND))
				{
					pCmd->buttons &= ~IN_JUMP;
				}
			}

			if (GetAsyncKeyState(VK_NUMPAD0)) Utils::ConsolePrint("%x\n", g_pLocalPlayer);

			Aimbot::Get().CreateMove(pCmd);

			//C_BaseEntity* vm = reinterpret_cast<C_BaseEntity*>(g_pLocalPlayer->m_hViewEntity()->Get());

			//if (!vm) {
			//	Utils::ConsolePrint("vm is NULL\n");
			//}
			//else {
			//	QAngle ang = vm->GetAbsAngles();
			//	Utils::ConsolePrint("%f %f %f\n", ang.pitch, ang.yaw, ang.roll);
			//}
		}

		qCurrentAngle = pCmd->view_angles;

		pVerified->m_cmd = *pCmd;
		pVerified->m_crc = pCmd->get_checksum();
	}

	void __fastcall hkFrameStageNotify(void* _this, int edx, ClientFrameStage_t stage)
	{
		static auto ofunc = client_hook.get_original<decltype(&hkFrameStageNotify)>(gmod_hook_indexes::FrameStageNotify);
		
		if (stage == ClientFrameStage_t::FRAME_RENDER_START) {
			if (g_pEngineClient && 
				g_pEngineClient->IsInGame() && 
				g_pLocalPlayer &&
				g_pLocalPlayer->get_health() > 0) {
				
				*g_pLocalPlayer->m_bDrawViewmodel() = o.bDrawViewmodel;
			}
		}
		
		ofunc(g_pClientDLL , edx, stage);
	}

	void __fastcall hkLockCursor(void* _this)
	{
		static auto ofunc = surface_hook.get_original<decltype(&hkLockCursor)>(gmod_hook_indexes::LockCursor);

		if (GmMenu::Get().m_bRunning)
			g_pInputSystem->EnableInput(!GmMenu::Get().m_bMenuOpen);
		else
			g_pInputSystem->EnableInput(true);

		if (g_pEngineClient->IsInGame() && 
			GmMenu::Get().m_bMenuOpen) {
			g_pSurface->unlock_cursor();
			return;
		}

		ofunc(g_pSurface);

	}

	// CBasePlayer::CalcView contains the code which adds the view punch to the view angles, by overriding the value before it's called
	// we can avoid having any viewpunch.
	int __fastcall hkCalcView(DWORD* thisptr, int edx, Vector& eyeOrigin, QAngle& eyeAngles, float& zNear, float& zFar, float& fov) {
		auto ofunc = (decltype(&hkCalcView))Hooks::TrueCalcView;

		Vector/*QAngle!!*/ ovPunchAngle = *g_pLocalPlayer->m_vecPunchAngle();
		if (!Aimbot::Get().bUseFASNoSpread) *g_pLocalPlayer->m_vecPunchAngle() = Vector(0, 0, 0);

		auto ret =  ofunc(thisptr, edx, eyeOrigin, eyeAngles, zNear, zFar, fov);

		if (!Aimbot::Get().bUseFASNoSpread) *g_pLocalPlayer->m_vecPunchAngle() = ovPunchAngle;

		return ret;
	}

	void __fastcall hkViewRender_Render(CViewRender* _this, int edx, vrect_t* rect) {
		static auto ofunc = viewrender_hook.get_original<decltype(&hkViewRender_Render)>(gmod_hook_indexes::ViewRender_Render);

		if (!(g_pEngineClient && g_pEngineClient->IsInGame() && g_pLocalPlayer && g_pLocalPlayer->is_alive())) {
			ofunc(_this, edx, rect);
			return;
		}
		
		ofunc(_this, edx, rect); // render
	}

	/* DX / WIN32 BULLSHIT */
	long __stdcall hkEndScene(IDirect3DDevice9* pDevice)
	{
		static auto oEndScene = direct3d_hook.get_original<decltype(&hkEndScene)>(gmod_hook_indexes::EndScene);

		/*
		IDirect3DVertexDeclaration9* vertDec;
		IDirect3DVertexShader9* vertShader;
		pDevice->GetVertexDeclaration(&vertDec);
		pDevice->GetVertexShader(&vertShader);
		*/
		DWORD colorwrite, srgbwrite;
		pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
		pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		//removes the source engine color correction
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);

		if (GmMenu::Get().m_bRunning && GmMenu::Get().m_bInitialised)
		{
			if (o.bDrawWaterMark) {
				GmMenu::Get().DrawWatermark();
			}

			if (o.bESPMasterEnabled) {
				ESP::Get().Render();
			}

			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			GmMenu::Get().Render();

			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		}

		
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
		/*
		pDevice->SetVertexDeclaration(vertDec);
		pDevice->SetVertexShader(vertShader);
		*/

		return oEndScene(pDevice);
	}

	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		static auto oReset = direct3d_hook.get_original<decltype(&hkReset)>(gmod_hook_indexes::Reset);

		pPresentationParameters->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		ImGui_ImplDX9_InvalidateDeviceObjects();
		ImGui_ImplDX9_CreateDeviceObjects();

		auto hr = oReset(device, pPresentationParameters);
		return hr;
	}

	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		
		const auto getButtonHeld = [uMsg, wParam](bool& bButton, int vKey)
		{
			if (wParam != vKey) return;

			if (uMsg == WM_KEYDOWN)
				bButton = true;
			else if (uMsg == WM_KEYUP)
				bButton = false;
		};

		const auto getButtonToggle = [uMsg, wParam](bool& bButton, int vKey)
		{
			if (wParam != vKey) return;

			if (uMsg == WM_KEYUP) {
				bButton = !bButton;
			}
		};

		getButtonToggle(GmMenu::Get().m_bMenuOpen, VK_F6);

		if (GmMenu::Get().m_bMenuOpen) {
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		}

		return CallWindowProcA(OriginalWNDProc, hWnd, uMsg, wParam, lParam);
	}
}