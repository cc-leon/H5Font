#pragma once

class HFFont : public CFont {
public:
    static BYTE CharSet;
    HFFont();
    virtual ~HFFont();

    BOOL CreateHFFont(LPCTSTR lpszFacenam, INT nHeight, int nWeight= FW_SEMIBOLD, BYTE bItalic=FALSE, BYTE bUnderline=FALSE);
    VOID GetUnicodes();
};

