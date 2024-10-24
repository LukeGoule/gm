#include "dx.h"

CFont::CFont(LPDIRECT3DDEVICE9 pDevice, int iFontSize, const char* pszTypeface) {
    this->m_pDevice     = pDevice;
    this->m_iFontSize   = iFontSize;
    this->m_pszTypeface = pszTypeface;

    D3DXCreateFontA(
        this->m_pDevice,
        iFontSize, 0,                   // h / w
        FW_NORMAL,                      // weight
        1,                              // MIP levels
        false,                          // italics
        DEFAULT_CHARSET,                // Charset
        OUT_DEFAULT_PRECIS,             // Output Precision
        ANTIALIASED_QUALITY,            // Quality
        DEFAULT_PITCH | FF_DONTCARE,    // Pitch & Family
        pszTypeface,                    // Typeface
        &this->m_pFont);
}

void CFont::Render(int x, int y, D3DCOLOR u32Color, CFont_Flags Flags, const char* pszFormat, ...) 
{
    if (!this->m_pFont) return;

    char buf[1024] = { '\0' };
    va_list va_alist;

    va_start(va_alist, pszFormat);
    vsprintf_s(buf, pszFormat, va_alist);
    va_end(va_alist);

    int tw = MeasureStringWidth(buf, this->m_pFont);
    LPRECT FontPos = new RECT{ x + 1, y, x + tw + 1, y + this->m_iFontSize };

    if (Flags & F_CENTERED) {
        x = x - (tw / 2);
    }

    if (Flags & F_DROPSHADOW) {
        this->m_pFont->DrawTextA(NULL, buf, -1, new RECT{ x+1, y+1, x + tw+1, y + this->m_iFontSize+1 }, DT_NOCLIP, 0xFF000000);
    }

    this->m_pFont->DrawTextA(NULL, buf, -1, FontPos, DT_NOCLIP, u32Color);
}

int CFont::MeasureStringWidth(const char* szText, void* pFont) {
    RECT rcRect = { 0,0,0,0 };
    if (pFont) {
        const auto font = (LPD3DXFONT)pFont;
        // calculate required rect
        font->DrawTextA(NULL, szText, strlen(szText), &rcRect, DT_CALCRECT,
            D3DCOLOR_XRGB(0, 0, 0));
    }

    // return width
    return rcRect.right - rcRect.left;
}

CDraw::CDraw() {}

CDraw::CDraw(LPDIRECT3DDEVICE9 pDevice)
{
    this->m_pDevice = pDevice;
}

CDraw::~CDraw() {

}

void CDraw::Box(int x, int y, int w, int h, D3DCOLOR Color)
{
    struct Vertex
    {
        float x, y, z, ht;
        DWORD Color;
    }
    V[4] = { {(float)x, (float)y + (float)h, 0.0f, 0.0f, Color}, { (float)x, (float)y, 0.0f, 0.0f, Color }, { (float)x + (float)w, (float)y + (float)h, 0.0f, 0.0f, Color }, { (float)x + (float)w, (float)y, 0.0f, 0.0f, Color } };

    this->m_pDevice->SetTexture(0, NULL);
    this->m_pDevice->SetPixelShader(0);
    this->m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    this->m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    this->m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    this->m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    this->m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    this->m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, V, sizeof(Vertex));
}

void CDraw::Triangle(int x0, int y0, int x1, int y1, int x2, int y2, DWORD Color) {
    struct Vertex
    {
        float x, y, z, ht;
        DWORD Color;
    }
    V[3] = { {(float)x0, (float)y0, 0.0f, 0.0f, Color}, { (float)x1, (float)y1, 0.0f, 0.0f, Color }, { (float)x2, (float)y2, 0.0f, 0.0f, Color } };
    
    LPDIRECT3DDEVICE9 pDevice = (LPDIRECT3DDEVICE9)this->m_pDevice;

    pDevice->SetTexture(0, NULL);
    pDevice->SetPixelShader(0);
    pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, V, sizeof(Vertex));
}

void CDraw::Line(int x0, int y0, int x1, int y1, DWORD Color)
{
    struct Vertex
    {
        float x, y, z, ht;
        DWORD Color;
    }
    V[4] = { {(float)x0, (float)y0, 0.0f, 0.0f, Color}, { (float)x1, (float)y1, 0.0f, 0.0f, Color } };

    LPDIRECT3DDEVICE9 pDevice = (LPDIRECT3DDEVICE9)this->m_pDevice;

    pDevice->SetTexture(0, NULL);
    pDevice->SetPixelShader(0);
    pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, V, sizeof(Vertex));
}