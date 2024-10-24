#include "reset.h"
#include "../hooks.h"

#pragma warning(push)
#pragma warning(disable:26495)
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning(pop)

#include "../../imgui/imgui_impl_dx9.h"

namespace Hooks
{
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		static auto oReset = direct3d_hook.get_original<decltype(&hkReset)>(Hooks::Indexes::IDirect3DDevice9::Reset);

		pPresentationParameters->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		ImGui_ImplDX9_InvalidateDeviceObjects();
		ImGui_ImplDX9_CreateDeviceObjects();

		auto hr = oReset(device, pPresentationParameters);
		return hr;
	}
}