#pragma once

#include "dxinc.h"

#include <iostream>
#include <iostream>
#include <intrin.h>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <TlHelp32.h>

#include "utils.h"
#include "NetVarManager.h"
#include "lua_source/lua.hpp"
#include "obfs.h"

#define DUMP_IFACE(classname, i)	_dump_iface(#classname, #i, (DWORD*)i)

#define CLIENT_DLL_VERSION			"VClient017"
#define VENGINECLIENT_VERSION		"VEngineClient"
#define VCLIENT_ENTITY_LIST_VERSION "VClientEntityList"
#define VGUI_SURFACE_VERSION		"VGUI_Surface030"
#define VENGINE_CVAR_VERSION		"VEngineCvar"
#define LUA_SHARED_VERSION			"LUASHARED"
#define GAMEEVENTS_MANAGER_VERSION  "GAMEEVENTSMANAGER002"
#define INPUT_SYSTEM_VERSION		"InputSystemVersion001"
#define MODEL_INFO_VERSION			"VModelInfoClient006"

#define CINPUT_PATTERN				"8B 0D ? ? ? ? 8B 01 FF 60 44"
#define D3DDEVICE_PATTERN			"A1 ? ? ? ? 50 8B 08 FF 51 0C"
#define CLIENTMODE_PATTERN			"8B 0D ? ? ? ? 8B 01 5D"
#define BEAMS_PATTERN				"89 86 ? ? ? ? 8B 0D ? ? ? ? 8B 01 FF 10" // https://www.unknowncheats.me/forum/counterstrike-global-offensive/240066-cs-beams-demystified.html

// SigMaker failed for me here so this was manually calculated. works tho :)
/*
pviewrender = 51 79 06 58
	  58 06 79 51                                              58 06 79 51
8B 0D ?? ?? ?? ?? 50 FF 57 7C 8B 4D F8 53 8B 01 FF 50 28 8B 0D ?? ?? ?? ?? 6A 00 8B 01 FF 50 7C
*/
#define VIEWRENDER_PATTERN			"8B 0D ?? ?? ?? ?? 50 FF 57 7C 8B 4D F8 53 8B 01 FF 50 28 8B 0D ?? ?? ?? ?? 6A 00 8B 01 FF 50 7C"

#define GLOBALVARS_PATTERN			"8B 0D ? ? ? ? 83 79 14 01 7E 12 6A 01 6A 01 E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 08"

#define CALCVIEW_PATTERN			"55 8B EC 53 56 8B F1 57 8B 8E 10 25 00 00 83 F9 FF 0F 84 ? ? ? ? 8B 15 ? ? ? ? 8B C1 25 FF 3F 00 00 C1 E9 0E 03 C0 39 4C C2 08 0F 85 ? ? ? ? 8B 4C C2 04 85 C9 0F 84 ? ? ? ? 8B 01 FF 90 5C 01 00 00 8B D8 85 DB 0F 84 ? ? ? ? 8B CE E8 ? ? ? ? D9 86 D4 25 00 00"

namespace SDK {

	void InitInterfaces();
	void DumpInterfaces();

	lua_State* LuaState();
}