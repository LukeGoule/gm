#include "xmods.h"
#include "hooks.h"
#include "sdk.h"
#include "vfunc_hook.h"
#include "utils.h"
#include "CInput.h"
#include "Netvars2.h"
#include "IVEngineClient.h"
#include "IGameEventManager.h"
#include "GameEvents.h"
#include "beams.h"
#include "ISurface.h"
#include "IInputSystem.h"
#include "globals.h"
#include "CInput.h"
#include "C_BaseEntity.h"
#include "md5.h"
#include "CViewRender.h"
#include "C_GMOD_Player.h"

#include "detours.h"

/* Features */
#include "aimbot.h"
#include "extrasensoryperception.h"
#include "recoil.h"

#include "luaShared.h"
#include "lua_source/lua.hpp"

extern "C" {
	#include "lua_source/lstate.h"
}

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

GameEvents* events;
bool obInGame = false;

namespace Hooks {

	using namespace std::literals::chrono_literals;

	vfunc_hook direct3d_hook;
	vfunc_hook client_hook;
	vfunc_hook surface_hook;
	vfunc_hook viewrender_hook;
	vfunc_hook localplayer_hook;
	vfunc_hook luainterface_hook; // luainterface client.

	HWND GMODWindow;
	WNDPROC OriginalWNDProc;

	uintptr_t* TrueCalcView;
	uintptr_t* TrueGetModelName;

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

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(GMODWindow);
		ImGui_ImplDX9_Init(g_pD3DDevice9);

		// Load singleton classes
		ESP		::Get().Setup();
		Aimbot	::Get().Setup();
		xmods	::Get().Setup();
		
		// Load netvars
		g_pNetvars = std::make_unique<NetvarTree>(); // doesn't werk
		g_NetworkedVariableManager = new CNetworkedVariableManager();

		if (direct3d_hook.setup(g_pD3DDevice9))
			Utils::ConsolePrint(_("[Init] D3D9 hooking setup.\n"));
		else
			Utils::ConsolePrint(_("[Init] g_pD3DDevice9 = 0x%x", g_pD3DDevice9));

		if (client_hook.setup(g_pClientDLL))
			Utils::ConsolePrint(_("[Init] BaseClient hooking setup.\n"));
		else
			Utils::ConsolePrint(_("[Init] g_pClientDLL = 0x%x", g_pClientDLL));

		if (surface_hook.setup(g_pSurface))
			Utils::ConsolePrint(_("[Init] Surface hooking setup.\n"));
		else
			Utils::ConsolePrint(_("[Init] g_pSurface = 0x%x", g_pSurface));

		if (viewrender_hook.setup(g_pViewRender))
			Utils::ConsolePrint(_("[Init] ViewRender hooking setup.\n"));
		else
			Utils::ConsolePrint(_("[Init] g_pViewRender = 0x%x", g_pViewRender));

		auto l_cl = g_pLuaShared->GetLuaInterface(LUAINTERFACE_CLIENT);
		if (luainterface_hook.setup(l_cl))
			Utils::ConsolePrint(_("[Init] ILuaInterface hooking setup.\n"));
		else
			Utils::ConsolePrint(_("[Init] interface = 0x%x", g_pLuaShared->GetLuaInterface(LUAINTERFACE_CLIENT)));

		direct3d_hook		.hook_index(gmod_hook_indexes::EndScene,				hkEndScene);			Utils::ConsolePrint(_("[Init] Hooked D3D9->EndScene           [%d]\n"), gmod_hook_indexes::EndScene);
		direct3d_hook		.hook_index(gmod_hook_indexes::Reset,					hkReset);				Utils::ConsolePrint(_("[Init] Hooked D3D9->Reset              [%d]\n"), gmod_hook_indexes::Reset);
		client_hook			.hook_index(gmod_hook_indexes::CreateMove,				hkCreateMove);			Utils::ConsolePrint(_("[Init] Hooked Client->CreateMove       [%d]\n"), gmod_hook_indexes::CreateMove);
		client_hook			.hook_index(gmod_hook_indexes::FrameStageNotify,		hkFrameStageNotify);	Utils::ConsolePrint(_("[Init] Hooked Client->FSN              [%d]\n"), gmod_hook_indexes::FrameStageNotify);
		surface_hook		.hook_index(gmod_hook_indexes::LockCursor,				hkLockCursor);			Utils::ConsolePrint(_("[Init] Hooked VGUISurface->LockCursor  [%d]\n"), gmod_hook_indexes::LockCursor);
		viewrender_hook		.hook_index(gmod_hook_indexes::ViewRender_Render,		hkViewRender_Render);	Utils::ConsolePrint(_("[Init] Hooked ViewRender->Render       [%d]\n"), gmod_hook_indexes::ViewRender_Render);
		luainterface_hook	.hook_index(gmod_hook_indexes::ILuaInterface_RunString, hkRunString);			Utils::ConsolePrint(_("[Init] Hooked luainterface->RunString  [%d]\n"), gmod_hook_indexes::ILuaInterface_RunString);
		Utils::ConsolePrint("[Init] Skipping LuaInterface->RunString!\n");
		TrueCalcView = (uintptr_t*)Utils::PatternScan(GetModuleHandleA("client.dll"), CALCVIEW_PATTERN);

		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		
		DetourAttach(&(PVOID&)TrueCalcView, Hooks::hkCalcView); Utils::ConsolePrint("Detours hooked CBasePlayer::CalcView\n");
		//DetourAttach(&(PVOID&)TrueGetModelName, Hooks::hkGetFOV);		Utils::ConsolePrint("Detours hooked C_PointCamera::GetFOV\n");

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
		//DetourDetach(&(PVOID&)TrueGetModelName, Hooks::hkGetFOV);
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
				g_pLocalPlayer->get_health() > 0) {
				
				*g_pLocalPlayer->m_bDrawViewmodel() = o.bDrawViewmodel;
			}
		}
		
		ofunc(g_pClientDLL , edx, stage);
	}

	void __fastcall hkLockCursor(void* _this)
	{
		static auto ofunc = surface_hook.get_original<decltype(&hkLockCursor)>(gmod_hook_indexes::LockCursor);

		if (xmods::Get().m_bRunning)
			g_pInputSystem->EnableInput(!xmods::Get().m_bMenuOpen);
		else
			g_pInputSystem->EnableInput(true);

		if (g_pEngineClient->IsInGame() && 
			xmods::Get().m_bMenuOpen) {
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

		if (xmods::Get().m_bRunning && xmods::Get().m_bInitialised)
		{
			if (o.bDrawWaterMark) {
				xmods::Get().DrawWatermark();
			}

			if (o.bESPMasterEnabled) {
				ESP::Get().Render();
			}

			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			xmods::Get().Render();

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

		getButtonToggle(xmods::Get().m_bMenuOpen, VK_F6);
		getButtonToggle(xmods::Get().m_bRunning, VK_F7);


		if (xmods::Get().m_bMenuOpen) {
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		}

		return CallWindowProcA(OriginalWNDProc, hWnd, uMsg, wParam, lParam);
	}
}