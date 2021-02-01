#pragma once
#include "HFBitmap.h"
#include "HFFont.h"

class HFMemDC : public CDC {
public:
    HFMemDC();
    virtual ~HFMemDC();
    BOOL CreateHFMemDC(
        CSize CONST& csDim, INT iStyleName,
        LPCTSTR lpszFacenam, INT nHeight, int nWeight = FW_SEMIBOLD, BYTE bItalic = FALSE, BYTE bUnderline = FALSE);
    CSize CONST& GetMemDCCSize() CONST;
    BOOL SaveToBMP(LPCTSTR bmFilename);

protected:
    HFBitmap m_bmp;
    HFFont m_font;
    CSize m_dim;
    int m_iStyle;
};

