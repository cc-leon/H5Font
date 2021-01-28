#pragma once
#include "HFBitmap.h"

class HFMemDC : public CDC {
public:
    HFMemDC();
    virtual ~HFMemDC();
    BOOL CreateBmpDC(CSize CONST& csDim, CFont& fntDevice);
    CSize GetDCCsize() CONST;
    BOOL SaveToBMP(LPCTSTR bmFilename);

protected:
    HFBitmap m_bmp;
};

