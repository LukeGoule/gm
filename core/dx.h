#pragma once

#include <iostream>
#include <Dwmapi.h> 
#include <TlHelp32.h>
#include <vector>

#define CONSOLAS 0

class CFont {
public:
    enum CFont_Flags {
        F_DROPSHADOW = 0x1,
        F_CENTERED   = 0x2
    };

private:
    void*          m_pFont         = nullptr;
    void*   m_pDevice       = nullptr;
    int                 m_iFontSize     = 0;
    const char*         m_pszTypeface   = nullptr;

public:

    CFont(void* pDevice, int iFontSize, const char* pszTypeface);

    void Render(int x, int y, DWORD u32Color, CFont_Flags Flags, const char* pszFormat, ...);

    static int MeasureStringWidth(const char* szText, void* pFont);
};

class CDraw {

    void*   m_pDevice = nullptr;

public:

    CDraw();
    CDraw(void* Device);
    ~CDraw();

    /* drawing */
    void                            Box             (int x, int y, int w, int h, DWORD Color);
    void                            Triangle        (int x0, int y0, int x1, int y1, int x2, int y2, DWORD Color);
    void                            Line            (int x0, int y0, int x1, int y1, DWORD Color);
};

extern CDraw* g_pDrawHelper;