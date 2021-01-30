#pragma once
#include "../H5FontCore/HFMemDC.h"

class HFImageDisplay : public CStatic {
protected:
    DWORD m_dwZoomValue;
    HFMemDC * m_pMemDC;

public:
    HFImageDisplay();
    virtual ~HFImageDisplay();
    BOOL CreateHFImageDisplay(CPoint CONST& ptTopLeft, CWnd* pParentWnd, HFMemDC* pMemDC=NULL, UINT nID = 0xffff);
    VOID LoadHFMemDC(HFMemDC * pMemDC);
    DWORD GetZoom() CONST;
    BOOL SetZoom(DWORD dwScale, CRect CONST& rcParentClient, CPoint CONST& ptCursor);
    CPoint DisplayToLogical(CPoint CONST& ptControl);
    WCHAR QueryCharUnicode(CPoint CONST& ptControl);
    CRect QueryCharCRect(CPoint CONST& ptControl);
    VOID QueryCharABC(CPoint CONST& ptControl, int* aiABC);

    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
};
