#pragma once

#ifndef __DX_H__
#define __DX_H__

#include <iostream>
#include <Dwmapi.h> 
#include <TlHelp32.h>
#include <vector>

#pragma warning(push)
#pragma warning(disable:26495)
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning(pop)

#define CONSOLAS 0

class CFont {
public:
    enum CFont_Flags {
        F_DROPSHADOW = 0x1,
        F_CENTERED   = 0x2
    };

private:
    LPD3DXFONT m_pFont = nullptr;
    LPDIRECT3DDEVICE9 m_pDevice = nullptr;
    int m_iFontSize = 0;
    const char* m_pszTypeface = nullptr;

public:

    CFont(LPDIRECT3DDEVICE9 pDevice, int iFontSize, const char* pszTypeface);

    void Render(int x, int y, D3DCOLOR u32Color, CFont_Flags Flags, const char* pszFormat, ...);

    static int MeasureStringWidth(const char* szText, void* pFont);
};


class CDraw {

    LPDIRECT3DDEVICE9 m_pDevice = nullptr;

public:

    CDraw();
    CDraw(LPDIRECT3DDEVICE9 Device);
    ~CDraw();

    void Box (int x, int y, int w, int h, D3DCOLOR Color);
    void Triangle (int x0, int y0, int x1, int y1, int x2, int y2, D3DCOLOR Color);
    void Line (int x0, int y0, int x1, int y1, D3DCOLOR Color);
};

extern CDraw* g_pDrawHelper;

#endif