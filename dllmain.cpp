#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "sdk.h"
#include "core/hooks.h"
#include "core/globals.h"
#include "menu/GMMenu.h"
#include "helpers/thread.h"

// Note: always include these lines prior to including any headers that use DX functionality.
#pragma warning(push)
#pragma warning(disable:26495)
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning(pop)

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"

#include "sdk/CHudChat.h"

template<class T>
inline T* FindHudElement(const char* name)
{
	if (!name)
		return nullptr;

	// SEARCH FOR CHudMenu TO FIND THIS.
	static auto pThis = *reinterpret_cast<DWORD**>(Utils::PatternScan(GetModuleHandleA("client.dll"), "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	if (!pThis)
		return nullptr;

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));

	if (!find_hud_element)
		return nullptr;

	return (T*)find_hud_element(pThis, name);
}


DWORD WINAPI OnDllAttach(PVOID base) {
	Utils::AttachConsole("gm");

	gm::SDK::Get().Initialise();

	Hooks::Init();

	while (GmMenu::Get().m_bRunning)
	{
		Hooks::CheckLua();
		Sleep(1000);
	}

	Hooks::Destroy();
	Utils::DetachConsole();

	FreeLibraryAndExitThread(HMODULE(base), 0);

	return TRUE;
}

VOID WINAPI OnDllDetach() {}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, OnDllAttach, 0, 0, 0);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		OnDllDetach();
	}
	return TRUE;
}