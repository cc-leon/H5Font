#include "pch.h"
#include "HFFont.h"

HFFont::HFFont() {}
HFFont::~HFFont() {}

BOOL HFFont::CreateHFFont(LPCTSTR lpszFacename, INT nHeight, int nWeight, BYTE bItalic, BYTE bUnderline) {
    return this->CreateFont(
        nHeight,  // int nHeight,
        0,        // int nWidth,
        0,        // int nEscapement,
        0,        // int nOrientation,
        nWeight,  // int nWeight,
        bItalic,  // BYTE bItalic,
        bUnderline, // BYTE bUnderline,
        FALSE, // BYTE cStrikeOut,
        ANSI_CHARSET, // BYTE nCharSet,
        OUT_DEVICE_PRECIS, // BYTE nOutPrecision,
        CLIP_DEFAULT_PRECIS,// BYTE nClipPrecision,
        CLEARTYPE_QUALITY,           // BYTE nQuality,
        DEFAULT_PITCH | FF_DONTCARE,  // BYTE nPitchAndFamily,
        lpszFacename); // LPCTSTR lpszFacename
}
