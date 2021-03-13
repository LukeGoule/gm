#include "imgui/imconfig.h"

const wchar_t* chartowchar(const char* s) {
    const WCHAR* pwcsName;
    // required size
    int nChars = MultiByteToWideChar(CP_ACP, 0, s, -1, NULL, 0);
    // allocate it
    pwcsName = new WCHAR[nChars];
    MultiByteToWideChar(CP_ACP, 0, s, -1, (LPWSTR)pwcsName, nChars);
    // use it....

    return pwcsName;
}