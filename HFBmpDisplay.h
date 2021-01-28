#pragma once
#include "HFBitmap.h"

class HFBmpDisplay : public CStatic {
protected:
    HFBitmap* m_bmpSource;
    DWORD m_dwZoomValue;

public:
    HFBmpDisplay();
    virtual ~HFBmpDisplay();
    HFBitmap * SetHFBitmap(HFBitmap * bmpNew);
    BOOL Create(CPoint CONST& ptTopLeft, HFBitmap* bmpNew, CWnd* pParentWnd, UINT nID = 0xffff);
    DWORD GetZoom() CONST;
    BOOL SetZoom(DWORD dwScale, CPoint CONST& ptCursor);

    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
