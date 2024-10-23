#include "utils.h"

#include "../core/options.h"
#include "../core/globals.h"

#include "../gmod/luaShared.h"
#include "../helpers/Color.h"
#include "../sdk/IVEngineClient.h"

#define CONSOLE_BLUE    1
#define CONSOLE_GREEN   2
#define CONSOLE_AQUA    3
#define CONSOLE_RED     4
#define CONSOLE_PURPLE  5
#define CONSOLE_GOLD    6
#define CONSOLE_GREY    8
#define CONSOLE_WHITE   15

std::uint8_t* Utils::PatternScan(void* module, const char* signature) {
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dosHeader = (PIMAGE_DOS_HEADER)module;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = pattern_to_byte(signature);
	auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scanBytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scanBytes[i];
		}
	}
	return nullptr;
}

DWORD Utils::find_pattern(DWORD start_offset, DWORD size, BYTE* pattern, const char mask[])
{
	DWORD pos = 0;
	int searchLen = strlen(mask) - 1;

	for (DWORD retAddress = start_offset; retAddress < start_offset + size; retAddress++)
	{
		if (*(BYTE*)retAddress == pattern[pos] || mask[pos] == '?') {
			if (mask[pos + 1] == '\0')
				return (retAddress - searchLen);
			pos++;
		}
		else
			pos = 0;
	}
	return NULL;
}


/*
Console shit.
The console will only be loaded if we are in debug mode, which only I should have access to, unless i self-"leak" or just give away the source.
*/

static HMODULE ourModule;
HANDLE _out = NULL, _old_out = NULL;
HANDLE _err = NULL, _old_err = NULL;
HANDLE _in = NULL, _old_in = NULL;

void Utils::AttachConsole(std::string title)
{
#ifdef _DEBUG
	_old_out = GetStdHandle(STD_OUTPUT_HANDLE);
	_old_err = GetStdHandle(STD_ERROR_HANDLE);
	_old_in = GetStdHandle(STD_INPUT_HANDLE);

	::AllocConsole() && ::AttachConsole(GetCurrentProcessId());

	_out = GetStdHandle(STD_OUTPUT_HANDLE);
	_err = GetStdHandle(STD_ERROR_HANDLE);
	_in = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(_out,
		ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

	SetConsoleMode(_in,
		ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
		ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);

	SetConsoleTitleA(title.c_str());
#endif
}

void Utils::DetachConsole()
{
#ifdef _DEBUG
	if (_out && _err && _in) {
		FreeConsole();

		if (_old_out)
			SetStdHandle(STD_OUTPUT_HANDLE, _old_out);
		if (_old_err)
			SetStdHandle(STD_ERROR_HANDLE, _old_err);
		if (_old_in)
			SetStdHandle(STD_INPUT_HANDLE, _old_in);
	}
#endif
}

bool Utils::_DirectConsolePrint(const char* buf) {
	if (!_out)
		return false;

	return !!WriteConsoleA(_out, buf, static_cast<DWORD>(strlen(buf)), nullptr, nullptr);
}

bool Utils::ConsolePrint(const char* fmt, ...)
{
	if (!_out)
		return false;

	char buf[1024];
	va_list va;

	va_start(va, fmt);
	_vsnprintf_s(buf, 1024, fmt, va);
	va_end(va);

#if _DEBUG

	SetConsoleTextAttribute(_out, CONSOLE_GREY);
	if (!Utils::_DirectConsolePrint(_("["))) return false;

	SetConsoleTextAttribute(_out, CONSOLE_AQUA);
	if (!Utils::_DirectConsolePrint(_("LOKI"))) return false;

	SetConsoleTextAttribute(_out, CONSOLE_GREY);
	if (!Utils::_DirectConsolePrint(_("] "))) return false;

	SetConsoleTextAttribute(_out, CONSOLE_WHITE);
	if (!Utils::_DirectConsolePrint(buf)) return false;
#endif

	
	if (o.bOutputLogToConsole && o.bFullyLoaded) {
		_InGameConsolePrint(Color(66, 134, 244, 200), _("%s"), buf);
	}
	
	return true;
}

bool __msg(Color clr, const char* txt) {
	if (!g_pEngineClient || !g_pLuaShared)
		return false;

	auto interface_type = LUAINTERFACE_CLIENT;
	if (!g_pEngineClient->IsInGame()) interface_type = LUAINTERFACE_MENU;


	g_pLuaShared->GetLuaInterface(interface_type)->MsgColour(clr, _("[LOKI] "));
	g_pLuaShared->GetLuaInterface(interface_type)->MsgColour(Color(255, 255, 255), _("%s"), txt);

	return true;
}

bool Utils::_InGameConsolePrint(Color clr, const char* fmt, ...) {
	if (!g_pEngineClient || !g_pLuaShared)
		return false;

	auto interface_type = LUAINTERFACE_CLIENT;
	if (!g_pEngineClient->IsInGame()) interface_type = LUAINTERFACE_MENU;

	char buf[1024];
	va_list va;

	va_start(va, fmt);
	_vsnprintf_s(buf, 1024, fmt, va);
	va_end(va);

	g_pLuaShared->GetLuaInterface(interface_type)->MsgColour(clr, _("[LOKI] "));
	g_pLuaShared->GetLuaInterface(interface_type)->MsgColour(Color(255, 255, 255), _("%s"), buf);

	return true;
}