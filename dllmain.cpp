#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "sdk.h"
#include "hooks.h"
#include "GMMenu.h"
#include "globals.h"
#include "thread.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"

#include "CHudChat.h"
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
	Utils::AttachConsole("");

	SDK::InitInterfaces();

#if _DEBUG
	SDK::DumpInterfaces();
#endif

	Hooks::Init();

	while (GmMenu::Get().m_bRunning);

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