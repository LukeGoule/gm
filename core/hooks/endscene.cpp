#include "endscene.h"
#include "../hooks.h"

#pragma warning(push)
#pragma warning(disable:26495)
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning(pop)

#include "../../menu/GmMenu.h"
#include "../../imgui/imgui_impl_dx9.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../hacks/esp.h"
#include "../options.h"

namespace Hooks 
{
	static void runEndSceneRender()
	{
		if (!GmMenu::Get().m_bRunning || !GmMenu::Get().m_bInitialised) return;

		// These preceed ImGui to prevent rendering on top of the menu.
		GmMenu::Get().DrawWatermark();
		ESP::Get().Render();

		// Start collecting ImGui buffer.
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		GmMenu::Get().Render();

		// Render ImGui
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	long __stdcall hkEndScene(IDirect3DDevice9* pDevice)
	{
		static auto oEndScene = direct3d_hook.get_original<decltype(&hkEndScene)>(Hooks::Indexes::IDirect3DDevice9::EndScene);

		// These commented out lines are required in CS:GO / CS2(?) to prevent texture glitches. Gmod seems to work fine without.
		/*
			IDirect3DVertexDeclaration9* vertDec;
			IDirect3DVertexShader9* vertShader;
			pDevice->GetVertexDeclaration(&vertDec);
			pDevice->GetVertexShader(&vertShader);
		*/

		// Store states so we can reset when we're done.
		DWORD colorwrite, srgbwrite;
		pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
		pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false); // Removes the source engine color correction

		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);

		runEndSceneRender();

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);

		/*
			pDevice->SetVertexDeclaration(vertDec);
			pDevice->SetVertexShader(vertShader);
		*/

		return oEndScene(pDevice);
	}
}