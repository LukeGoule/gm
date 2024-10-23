#pragma once

#include <Windows.h>
#include <string>
#include <vector>

class Color;

namespace Utils {

    void Msg(std::string str);
    std::uint8_t* PatternScan(void* module, const char* signature);

    DWORD find_pattern(DWORD start_offset, DWORD size, BYTE* pattern, const char mask[]);

    void AttachConsole(std::string title);
    void DetachConsole();
    bool _DirectConsolePrint(const char* buf);
    bool ConsolePrint(const char* fmt, ...);
    bool _InGameConsolePrint(Color clr, const char* fmt, ...);

    void TitleBanner();
}