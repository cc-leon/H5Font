#pragma once
#include "HFBmpDisplay.h"

class HFDrawWindow : public CFrameWnd {
protected:
    HFBmpDisplay * m_dispImage;
    CPoint m_ptPrevCursor;
    HCURSOR m_hPrevCursor;

public:
    HFDrawWindow();           // protected constructor used by dynamic creation
    virtual ~HFDrawWindow();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    DECLARE_MESSAGE_MAP()
    afx_msg void OnClose();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
};
