#include "pch.h"

#include "HFMemDC.h"
#include "../H5FontCore/HFBitMask.h"

HFMemDC::HFMemDC() 
    : CDC(), m_awcUnicodes(NULL), m_abcUnicodes(NULL), m_cwcUnicodes(0)
    , m_fontinfo{0}, m_iStyle(0), m_ptUnicodes(NULL) {}
HFMemDC::~HFMemDC() {}

BOOL HFMemDC::CreateHFMemDC(FONTINFO CONST& fontinfo, int iStyle) {

    CString sLog;

    m_iStyle = iStyle;
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
        DEFAULT_CHARSET,              // CharSet
        OUT_DEVICE_PRECIS,         // OutPrecision
        CLIP_DEFAULT_PRECIS,       // ClipPrecision
        ANTIALIASED_QUALITY,         // Quality
        FIXED_PITCH || FF_MODERN,  // PitchAndFamily
        fontinfo.szFacenam);       // Facename
    SelectObject(m_font);
    if (!__subGetAllUnicodes()) {
        return FALSE;
    }

    int dimW = ((int)sqrt(m_cwcUnicodes) + 1) * (fontinfo.nHeight) + fontinfo.nHeight * 2;
    int dimH;
    for (int i = 1; i <= 0xffff; i <<= 1) {
        if (i > dimW) {
            if (dimW * dimW < i * (i >> 1)) {
                dimH = i / 2;
            }
            else {
                dimH = i;
            }
            dimW = i;
            break;
        }
    }
    if (dimW <= 0x1000) {
        m_dim = CSize(dimW, dimH);
        sLog.Format(HFSTRC(IDS_LOG_DIMENSION_REQUIRED), fontinfo.nHeight, fontinfo.nWeight, dimW, dimH);
        LOG.log(sLog, LOG.INFO);
    }
    else {
        sLog.Format(HFSTRC(IDS_LOG_DIMENSION_EXCEEDED), dimW, 0x1000);
        LOG.log(sLog, LOG.INFO);
        return FALSE;
    }

    //
    // Step 2. Create bmp with right size, fill masking color and set all the right color
    //
    m_image.Destroy();
    m_image.Create(dimW, dimH, 32);
    SelectObject(m_image);
    SetTextColor(FONT_COLOR);
    SetBkColor(BKGD_COLOR);
    SetBkMode(OPAQUE);

    //
    // Step 4. Draw Characters
    //
    mem::FreeMem(m_ptUnicodes);
    m_ptUnicodes = mem::GetMem<POINT>(m_cwcUnicodes);

    CPoint ptCurr(fontinfo.nHeight, +fontinfo.nHeight);
    for (size_t i = 0; i < m_cwcUnicodes; i++) {
        m_ptUnicodes[i] = ptCurr;
        ptCurr = __subDrawUnicode(ptCurr, i, fontinfo.nHeight);
    }
    LOG.log(HFSTRC(IDS_LOG_SAVING_DC_DONE), LOG.INFO);
    return TRUE;
}

CSize CONST& HFMemDC::GetMemDCCSize() CONST {
    return m_dim;
}


size_t HFMemDC::GetUnicodeCount() CONST {
    return m_cwcUnicodes;
}

FONTINFO CONST* HFMemDC::GetFontInfo() CONST {
    return &m_fontinfo;
}

size_t HFMemDC::FillUNICODEINFO(size_t iIndex, LPUNICODEINFO puiCurr) {
    if (iIndex >= m_cwcUnicodes) {
        return 0;
    }

    int a = m_abcUnicodes[iIndex].abcA;
    int b = m_abcUnicodes[iIndex].abcB;
    int c = m_abcUnicodes[iIndex].abcC;

    puiCurr->wcUnicode = m_awcUnicodes[iIndex];
    int lrOffset = 0;
    if (sys::info.GetPaddingCharSet()->GetBit(puiCurr->wcUnicode)) {
        lrOffset = m_fontinfo.nPadding;
    }
    puiCurr->aiPos[UNICODEINFO::L_BOUND] = m_ptUnicodes[iIndex].x;
    puiCurr->aiPos[UNICODEINFO::R_BOUND] = m_ptUnicodes[iIndex].x + abs(a) + abs(b) + abs(c);
    puiCurr->aiPos[UNICODEINFO::T_BOUND] = m_ptUnicodes[iIndex].y;
    puiCurr->aiPos[UNICODEINFO::B_BOUND] = m_ptUnicodes[iIndex].y + m_fontinfo.nHeight;

    puiCurr->aiPos[UNICODEINFO::L_OFFSET] = 0;
    puiCurr->aiPos[UNICODEINFO::R_OFFSET] = a + b + c + lrOffset;
    if (a < 0) {
        puiCurr->aiPos[UNICODEINFO::L_OFFSET] += a;
    }

    puiCurr->aiPos[UNICODEINFO::UNKNOWN] = 0;

    return iIndex + 1;
}

BOOL HFMemDC::SaveDDS(LPCTSTR filename) {
    CString sLog;
    CString sPng = filename;
    sPng.Delete(sPng.GetLength() - 3, 3);
    sPng.Append(_T("png"));
    sLog.Format(HFSTRC(IDS_LOG_SAVING_PNG), sPng);
    LOG.log(sLog, LOG.INFO);
    if (m_image.Save(sPng) != S_OK) {
        sLog.Format(HFSTRC(IDS_LOG_SAVING_PNG_ERROR), sPng);
        LOGUSR(sLog);
        return FALSE;
    }

    sLog.Format(HFSTRC(IDS_LOG_SAVE_DDS_FILE), filename);
    LOG.log(sLog, LOG.INFO);
    CString sCmd;
    sCmd.Format(
        _T("%s convert %s -alpha copy -define dds:mipmaps=0 -define dds:compression=none %s"),
        HFFC::exe::MAGICK_CMD, (LPCTSTR)sPng, filename);
    LOG.log(HFSTRC(IDS_LOG_NOW_EXECUTING), LOG.STD, TRUE);
    LOG.log(sCmd, LOG.STD);
    DWORD dwExitCode = 0;
    LOG.log(sys::RunExe(sCmd, &dwExitCode), LOG.STD);
    if (dwExitCode) {
        sLog.Format(HFSTRC(IDS_LOG_WRONG_EXIT_CODE), dwExitCode);
        LOGUSR(sLog);
        return FALSE;
    }

    if (!::DeleteFile(sPng)) {
        sLog.Format(HFSTRC(IDS_LOG_DELETING_TEMP_ERROR), sPng);
        LOGUSR(sLog);
        return FALSE;
    }
    LOG.log(HFSTRC(IDS_LOG_FINISHED), LOG.NORM, TRUE);
    return TRUE;
}

BOOL HFMemDC::__subGetAllUnicodes() {
    CString sTemp;
    DWORD cbgsSize = ::GetFontUnicodeRanges(*this, NULL);

    LPGLYPHSET lpgsCurr = mem::GetMem<GLYPHSET>(cbgsSize);
    ::GetFontUnicodeRanges(*this, lpgsCurr);
    if (lpgsCurr->flAccel == GS_8BIT_INDICES) {
        sTemp.Format(HFSTRC(IDS_LOG_FONT_FILE_NOT_SUPPORTED));
        LOG.log(sTemp, LOG.INFO);
        return FALSE;
    }

    size_t cwcFont = lpgsCurr->cGlyphsSupported;
    LPWSTR awcFont = mem::GetMem<WCHAR>(cwcFont);

    size_t i = 0;
    for (DWORD j = 0; j < lpgsCurr->cRanges; j++) {
        for (USHORT k = 0; k < lpgsCurr->ranges[j].cGlyphs; k++) {
            awcFont[i] = lpgsCurr->ranges[j].wcLow + k;
            i++;
        }
    }

    LPABC abcFont = mem::GetMem<ABC>(cwcFont);

    i = 0;
    for (DWORD j = 0; j < lpgsCurr->cRanges; j++) {
        ::GetCharABCWidths(
            *this, lpgsCurr->ranges[j].wcLow,
            lpgsCurr->ranges[j].wcLow + lpgsCurr->ranges[j].cGlyphs - 1,
            &abcFont[i]);
        i += lpgsCurr->ranges[j].cGlyphs;
    }

    sTemp.Format(HFSTRC(IDS_LOG_NUM_UNICODES_ABC_READ), lpgsCurr->cGlyphsSupported, i);
    LOG.log(sTemp, LOG.INFO);

    LPABC* pabcUnicodes = mem::GetMem<LPABC>(0xFFFF);
    ::ZeroMemory(pabcUnicodes, sizeof(LPABC) * 0xFFFF);

    for (size_t j = 0; j < cwcFont; j++) {
        pabcUnicodes[(size_t)awcFont[j]] = &abcFont[j];
    }

    HFBitMask bmGB2312(0xFFFF);
    size_t cwcGB2312 = sys::info.FillUnicodes(NULL);
    LPWSTR awcGB2312 = mem::GetMem<WCHAR>(cwcGB2312);
    sys::info.FillUnicodes(awcGB2312);
    for (size_t j = 0; j < cwcGB2312; j++) {
        bmGB2312[(size_t)awcGB2312[j]] = TRUE;
    }

    m_cwcUnicodes = 0;
    for (size_t j = 0; j < 0xFFFF; j++) {
        if (bmGB2312[j] && (pabcUnicodes[j] != NULL)) {
            m_cwcUnicodes++;
        }
        else if (!bmGB2312[j] && (pabcUnicodes[j] != NULL)) {
            pabcUnicodes[j] = NULL;
        }
    }

    mem::FreeMem(m_awcUnicodes);
    mem::FreeMem(m_abcUnicodes);
    m_awcUnicodes = mem::GetMem<WCHAR>(m_cwcUnicodes);
    m_abcUnicodes = mem::GetMem<ABC>(m_cwcUnicodes);

    i = 0;
    for (size_t j = 0; j < 0xFFFF; j++) {
        if (pabcUnicodes[j] != NULL) {
            m_awcUnicodes[i] = (WCHAR)j;
            m_abcUnicodes[i] = *pabcUnicodes[j];
            i++;
        }
    }

    sTemp.Format(HFSTRC(IDS_LOG_NUM_UNICODES_FILTERED), m_cwcUnicodes);
    LOG.log(sTemp, LOG.INFO);

    mem::FreeMem(lpgsCurr);
    mem::FreeMem(awcFont);
    mem::FreeMem(abcFont);
    mem::FreeMem(awcGB2312);
    mem::FreeMem(pabcUnicodes);
    return TRUE;
}

CPoint HFMemDC::__subDrawUnicode(CPoint CONST& ptCurr, size_t iIndex, int iHeight) {
    WCHAR wszOutput[2];
    wszOutput[0] = m_awcUnicodes[iIndex];
    wszOutput[1] = 0;
    ::TextOutW(*this, ptCurr.x, ptCurr.y, wszOutput, 1);

    CPoint ptResult = ptCurr;
    ptResult.x += abs(m_abcUnicodes[iIndex].abcA);
    ptResult.x += m_abcUnicodes[iIndex].abcB;
    ptResult.x += abs(m_abcUnicodes[iIndex].abcC);
    LONG iRight = m_dim.cx;

    if (iIndex < m_cwcUnicodes - 1 && LONG(ptResult.x + m_fontinfo.nHeight * 2) > iRight) {
        ptResult.x = m_fontinfo.nHeight;
        ptResult.y += iHeight;
    }

    return ptResult;
}
