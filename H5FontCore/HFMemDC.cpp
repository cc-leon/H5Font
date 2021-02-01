#include "pch.h"
#include "HFMemDC.h"
HFMemDC::HFMemDC() : CDC() {}
HFMemDC::~HFMemDC() {}

/*
CDC memdc;
memdc.CreateCompatibleDC(NULL);
CBitmap freshMap;
freshMap.CreateBitmap(1000, 1000, 1, 32, NULL);
CBitmap* oldmap = memdc.SelectObject(&freshMap);
memdc.SetBkMode(TRANSPARENT);
memdc.SetTextColor(UIConst::Color::WHITE);
CFont newFont;
newFont.CreateFont(
    30,                            // Height
    0,                             // Width
    0,                             // Escapement
    0,                             // OrientationME
    FW_BOLD,                       // Weight
    FALSE,                         // Italic
    FALSE,                          // Underline
    0,                             // StrikeOut
    ANSI_CHARSET,                  // CharSet
    OUT_DEVICE_PRECIS,            // OutPrecision
    CLIP_DEFAULT_PRECIS,           // ClipPrecision
    CLEARTYPE_QUALITY,               // Quality
    FIXED_PITCH | FF_ROMAN,      // PitchAndFamily
    _T("Monotype Corsiva"));      // Facename
memdc.SelectObject(newFont);
CString aa(_T("Heroes of might and magic V"));
memdc.ExtTextOut(0, 0, ETO_CLIPPED, NULL, aa, NULL);

CRect dispRect;
GetClientRect(dispRect);
CSize bmpSize = m_bmpSource->GetBmpCSize();
dc.StretchBlt(0, 0, dispRect.Width(), dispRect.Height(), &memdc, 0, 0, bmpSize.cx, bmpSize.cy, SRCCOPY);
*/


BOOL HFMemDC::CreateHFMemDC(
    CSize CONST& csDim, INT iStyleName,
    LPCTSTR lpszFacenam, INT nHeight, int nWeight, BYTE bItalic, BYTE bUnderline) {
    m_font.CreateHFFont(lpszFacenam, nHeight, nWeight, bItalic, bUnderline);
    m_dim = csDim;
    m_iStyle = iStyleName;

    CreateCompatibleDC(NULL);
    CBitmap freshMap;
    freshMap.CreateBitmap(csDim.cx, csDim.cy, 1, 32, NULL);
    SelectObject(freshMap);

    CBrush redBrush;
    redBrush.CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(redBrush);
    freshMap.DeleteObject();

    SetBkMode(OPAQUE);
    SetTextColor(RGB(255,255,255));
    SetBkColor(RGB(0, 0, 0));

    TextOut(10, 10, _T("!@#$%^&*()"));

    return FALSE;
}

CSize CONST& HFMemDC::GetMemDCCSize() CONST {
    return m_dim;
}

BOOL HFMemDC::SaveToBMP(LPCTSTR bmFilename) {
    return FALSE;
}
