#pragma once

class HFMemDC : public CDC {
public:
    static COLORREF CONST PLATE_COLOR = RGB(255, 0, 0);
    static COLORREF CONST FONT_COLOR = RGB(255, 255, 255);
    static COLORREF CONST BKGD_COLOR = RGB(0, 0, 0);

    static int CONST PADDING = 0;
    static size_t CONST EOUNICODES = 0xFFFF + 1;

    HFMemDC();
    virtual ~HFMemDC();
    BOOL CreateHFMemDC(FONTINFO CONST& fontinfo, int iStyle);
    CSize CONST& GetMemDCCSize() CONST;
    size_t GetUnicodeCount() CONST;
    size_t FillUNICODEINFO(size_t iIndex, LPUNICODEINFO puiCurr);
    VOID SaveDDS(LPCTSTR filename);

protected:
    CImage m_image;
    CFont m_font;
    FONTINFO m_fontinfo;
    int m_iStyle;
    CSize m_dim;
    LPWSTR m_awcUnicodes;
    LPABC m_abcUnicodes;
    size_t m_cwcUnicodes;
    LPPOINT m_ptUnicodes;

    VOID __subGetAllUnicodes();
    CPoint __subDrawUnicode(CPoint CONST& ptCurr, size_t iIndex, int iHeight);
};

