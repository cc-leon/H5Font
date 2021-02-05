#include "pch.h"
#include "HFMemDC.h"

HFMemDC::HFMemDC() 
    : CDC(), m_awcUnicodes(NULL), m_abcUnicodes(NULL), m_cwcUnicodes(0)
    , m_fontinfo{0}, m_ptUnicodes(NULL) {}
HFMemDC::~HFMemDC() {}

BOOL HFMemDC::CreateHFMemDC(FONTINFO CONST& fontinfo) {

    CString sLog;

    DeleteDC();
    CreateCompatibleDC(NULL);

    //
    // Step 1. Create font and gather unicode ranges, thus deciding dimension
    //

    m_fontinfo = fontinfo;
    m_font.DeleteObject();
    m_font.CreateFont(
        fontinfo.nHeight,          // Height
        0,                         // Width
        0,                         // Escapement
        0,                         // OrientationME
        fontinfo.nWeight,          // Weight
        fontinfo.bItalic,          // Italic
        fontinfo.bUnderline,       // Underline
        0,                         // StrikeOut
        ANSI_CHARSET,              // CharSet
        OUT_DEVICE_PRECIS,         // OutPrecision
        CLIP_DEFAULT_PRECIS,       // ClipPrecision
        CLEARTYPE_QUALITY,         // Quality
        FIXED_PITCH || FF_MODERN,  // PitchAndFamily
        fontinfo.szFacenam);       // Facename
    SelectObject(m_font);
    __subGetAllUnicodes();

    int dim = ((int)sqrt(m_cwcUnicodes) + 1) * (fontinfo.nHeight + fontinfo.nPadding);
    for (int i = 1; i < 0xffff; i <<= 1) {
        if (i > dim) {
            dim = i;
            break;
        }
    }
    m_dim = CSize(dim, dim);
    sLog.Format(HFSTRC(IDS_LOG_DIMENSION_REQUIRED), fontinfo.nHeight, fontinfo.nWeight, dim, dim);
    LOG.log(sLog, RGB(0,255,0));

    //
    // Step 2. Create bmp with right size, fill masking color and set all the right color
    //
    m_image.Destroy();
    m_image.Create(dim, dim, 32);
    SelectObject(m_image);
    CPen pen; CBrush brush;
    pen.CreatePen(PS_SOLID, 1, PLATE_COLOR);
    brush.CreateSolidBrush(PLATE_COLOR);
    SelectObject(pen);
    SelectObject(brush);
    CRect rect(0, 0, m_dim.cx, m_dim.cy);
    Rectangle(rect);

    SetTextColor(FONT_COLOR);
    SetBkColor(BKGD_COLOR);
    SetBkMode(OPAQUE);

    //
    // Step 4. Draw Characters
    //
    CPoint ptCurr(0, 0);
    for (size_t i = 0; i < m_cwcUnicodes; i++) {
        m_ptUnicodes[i] = ptCurr;
        ptCurr = __subDrawUnicode(ptCurr, i, fontinfo.nHeight);
    }

    m_image.Save(_T("D:\\aa.png"));
    return TRUE;
}

CSize CONST& HFMemDC::GetMemDCCSize() CONST {
    return m_dim;
}


size_t HFMemDC::GetUnicodeCount() CONST {
    return m_cwcUnicodes;
}

size_t HFMemDC::FillUNICODEINFO(size_t iIndex, LPUNICODEINFO puiCurr) {
    if (iIndex >= m_cwcUnicodes) {
        return EOUNICODES;
    }

    int a = m_abcUnicodes[iIndex].abcA;
    int b = m_abcUnicodes[iIndex].abcB;
    int c = m_abcUnicodes[iIndex].abcC;

    puiCurr[iIndex].wcUnicode = m_awcUnicodes[iIndex];
    puiCurr[iIndex].aiPos[UNICODEINFO::L_BOUND] = m_ptUnicodes[iIndex].x;
    puiCurr[iIndex].aiPos[UNICODEINFO::R_BOUND] = m_ptUnicodes[iIndex].x + 20;
    puiCurr[iIndex].aiPos[UNICODEINFO::B_BOUND] = m_ptUnicodes[iIndex].y + 20;
    puiCurr[iIndex].aiPos[UNICODEINFO::L_OFFSET] = 0;
    puiCurr[iIndex].aiPos[UNICODEINFO::R_OFFSET] = 0;
    puiCurr[iIndex].aiPos[UNICODEINFO::B_OFFSET] = 0;
    puiCurr[iIndex].aiPos[UNICODEINFO::UNKNOWN] = 0;

    return iIndex + 1;
}

VOID HFMemDC::__subGetAllUnicodes() {
    CString sTemp;
    DWORD cbgsSize = ::GetFontUnicodeRanges(*this, NULL);

    LPGLYPHSET lpgsCurr = mem::GetMem<GLYPHSET>(cbgsSize);
    ::GetFontUnicodeRanges(*this, lpgsCurr);
    sTemp.Format(HFSTRC(IDS_LOG_NUM_UNICODES_IN_FONT), lpgsCurr->cGlyphsSupported);
    LOG.log(sTemp, RGB(0,255,0));
    if (lpgsCurr->flAccel == GS_8BIT_INDICES) {
        sTemp.Format(HFSTRC(IDS_LOG_FONT_FILE_NOT_SUPPORTED));
        LOG.log(sTemp);
    }

    m_cwcUnicodes = lpgsCurr->cGlyphsSupported;
    mem::FreeMem(m_awcUnicodes);
    m_awcUnicodes = mem::GetMem<WCHAR>(m_cwcUnicodes);

    size_t i = 0;
    for (DWORD j = 0; j < lpgsCurr->cRanges; j++) {
        for (USHORT k = 0; k < lpgsCurr->ranges[j].cGlyphs; k++) {
            m_awcUnicodes[i] = lpgsCurr->ranges[j].wcLow + k;
            i++;
        }
    }

    sTemp.Format(HFSTRC(IDS_LOG_NUM_UNICODES_READ), i);
    LOG.log(sTemp, RGB(0, 255, 0));

    mem::FreeMem(m_abcUnicodes);
    m_abcUnicodes = mem::GetMem<ABC>(m_cwcUnicodes);

    i = 0;
    for (DWORD j = 0; j < lpgsCurr->cRanges; j++) {
        ::GetCharABCWidths(
            *this, lpgsCurr->ranges[j].wcLow,
            lpgsCurr->ranges[j].wcLow + lpgsCurr->ranges[j].cGlyphs - 1,
            &m_abcUnicodes[i]);
        i += lpgsCurr->ranges[j].cGlyphs;
    }

    sTemp.Format(HFSTRC(IDS_LOG_NUM_ABC_READ), i);
    LOG.log(sTemp, RGB(0, 255, 0));

    for (i = 32; i < 80; i++) {
        //sTemp.Format(_T("%3d, 0x%04x, '%c', %3d, %3d, %3d"), i, m_awcUnicodes[i], m_awcUnicodes[i], m_abcUnicodes[i].abcA, m_abcUnicodes[i].abcB, m_abcUnicodes[i].abcC);
        //LOG.log(sTemp);
    }
    BOOL bj = ::GetMapMode(*this) == MM_TEXT;
    UINT uui = GetTextAlign();

    mem::FreeMem(m_ptUnicodes);
    m_ptUnicodes = mem::GetMem<POINT>(m_cwcUnicodes);
    mem::FreeMem(lpgsCurr);

}

CPoint HFMemDC::__subDrawUnicode(CPoint CONST& ptCurr, size_t iIndex, int iHeight) {
    WCHAR wszOutput[2];
    wszOutput[0] = m_awcUnicodes[iIndex];
    wszOutput[1] = 0;
    ::TextOutW(*this, ptCurr.x, ptCurr.y, wszOutput, 1);

    CPoint ptResult = ptCurr;
    ptResult.x += PADDING;
    ptResult.x += abs(m_abcUnicodes[iIndex].abcA);
    ptResult.x += m_abcUnicodes[iIndex].abcB;
    ptResult.x += abs(m_abcUnicodes[iIndex].abcC);
    LONG iRight = m_dim.cx;

    if (iIndex < m_cwcUnicodes - 1 && LONG(ptResult.x + m_abcUnicodes[iIndex].abcB) >= iRight) {
        ptResult.x = 0;
        ptResult.y += PADDING + iHeight;
    }

    return ptResult;
}
