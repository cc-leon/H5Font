#pragma once
#include "HFImageDisplay.h"
#include "HFDrawWindowInfoBar.h"
#include "../H5FontCore/HFMemDC.h"

class HFDrawWindowContainer : public CWnd {
protected:
    HFImageDisplay* m_dispImage;
    HFDrawWindowInfoBar* m_infobar;
    CPoint m_ptPrevCursor;
    HCURSOR m_hPrevCursor;

    BOOL RegisterThisClass();

public:
    HFDrawWindowContainer();
    virtual ~HFDrawWindowContainer();
    
    BOOL CreateHFDrawWindowContainer(CWnd* pParentWnd, HFDrawWindowInfoBar * pInfoBar, UINT nID=HFUIC::DrawWindow::ID_container);
    VOID OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    VOID RestoreZoom();
    CPoint GetImageCenter();
    VOID SetMemDC(HFMemDC * memDC);

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    DECLARE_MESSAGE_MAP()

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
