#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "sdk.h"
#include "hooks.h"
#include "xmods.h"
#include "globals.h"
#include "loader_dll_shared.h" //"../shared/loader_dll_shared.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"

/* radio */
//#include "bass.h"
//#include "dll.h"
//#include "api.h"
//#include "dll_image.h"
#include "radiomanager.h"

char Username[256]	= { USERNAME_FINDER_DEFAULT_BYTES  USERNAME_FINDER_DEFAULT_BYTES };
char Password[256]	= { PASSWORD_FINDER_DEFAULT_BYTES  PASSWORD_FINDER_DEFAULT_BYTES };
char MachineID[256] = { MACHINEID_FINDER_DEFAULT_BYTES MACHINEID_FINDER_DEFAULT_BYTES };

auto bass_needs_reinit = false;
using namespace std::chrono;

void SilentCrash()
{
	// Need a better solution if porting to x64.
	__asm
	{
		rdtsc
		XOR edx, eax
		add eax, edx
		mov esp, eax
		XOR ebp, edx
		mov ebx, ebp
		mov ecx, esp
		XOR esi, ebx
		XOR edi, esp
		jmp eax
	}
}

const char* GetLoaderUserName() {
	return Username;
}

const char* GetLoaderUserPassword() {
	return Password;
}

// "HWID", probably spoofable.
const char* GetLoaderUserKey() {
	return MachineID;
}

void LoaderCheck() {
#ifdef _RELEASE

#error "Disable this section!"

	// Something in the loader is really broken.
	if (!Username || !MachineID || !Password) {
		SilentCrash();
	}

	if (!strcmp(Username, USERNAME_FINDER_DEFAULT) ||
		!strcmp(Password, PASSWORD_FINDER_DEFAULT) ||
		!strcmp(MachineID, MACHINEID_FINDER_DEFAULT)) {
		MessageBox(0, "bad data", "", 0);
		SilentCrash();
	}

#endif
}

void UpdateBASS() {
	/*std::this_thread::sleep_for(100ms);
	const auto desired_channel = g_pRadio->m_iRadioChannel;

	if (BASS::bass_init && desired_channel)
	{
		static auto current_channel = 0;

		if (current_channel != desired_channel || bass_needs_reinit)
		{
			bass_needs_reinit = false;
			BASS_Start();

			Utils::ConsolePrint(_("[BASS] Now playing %s\n"), g_pRadio->GetCurrentlyPlaying()->name.c_str());
			BASS_OPEN_STREAM(g_pRadio->GetCurrentlyPlaying()->url.c_str());
			
			current_channel = desired_channel;
		}

		BASS_SET_VOLUME(BASS::stream_handle, (float)g_pRadio->m_iRadioVolumePercent / 100.f);
		BASS_PLAY_STREAM();
	}
	else if (BASS::bass_init)
	{
		bass_needs_reinit = true;
		BASS_StreamFree(BASS::stream_handle);
	}*/
}

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
	LoaderCheck();

	Utils::AttachConsole("");

	SDK::InitInterfaces();
	SDK::DumpInterfaces();
	Hooks::Init();

	//BASS::bass_lib_handle = BASS::bass_lib.LoadFromMemory(bass_dll_image, sizeof(bass_dll_image));

	//if (BASS_INIT_ONCE()) BASS::bass_init = TRUE;

	while (xmods::Get().m_bRunning) {

		//if (GetAsyncKeyState(VK_F9)) {
		//	CHudChat* chudchat = FindHudElement<CHudChat>("CHudChat");
		//	if (chudchat) {
		//		chudchat->ChatPrintf(0, CHudChat::CHAT_FILTER_NONE, "Bruh moments!\n");
		//	}
		//	else {
		//		Utils::ConsolePrint("couldn't find CHudChat\n");
		//	}
		//}

		//UpdateBASS();
	}

	//BASS_STOP_STREAM();
	//BASS_Stop();
	//BASS_Free();

	Hooks::Destroy();
	Utils::DetachConsole();

	FreeLibraryAndExitThread(HMODULE(base), 0);

	return TRUE;
}

VOID WINAPI OnDllDetach() {

}


#define THREAD_CREATE_FLAGS_CREATE_SUSPENDED 0x00000001
#define THREAD_CREATE_FLAGS_SKIP_THREAD_ATTACH 0x00000002
#define THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER 0x00000004
#define THREAD_CREATE_FLAGS_HAS_SECURITY_DESCRIPTOR 0x00000010
#define THREAD_CREATE_FLAGS_ACCESS_CHECK_IN_TARGET 0x00000020
#define THREAD_CREATE_FLAGS_INITIAL_THREAD 0x00000080

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