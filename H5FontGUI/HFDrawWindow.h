#pragma once
#include "HFDrawWindowContainer.h"
#include "HFDrawWindowInfoBar.h"

class HFDrawWindow : public CFrameWnd {
protected:
    HFDrawWindowInfoBar m_infoBar;
    HFDrawWindowContainer m_container;
    CMenu m_mnDrawWindow;

public:
    HFDrawWindow();           // protected constructor used by dynamic creation
    virtual ~HFDrawWindow();
    BOOL CreateHFDrawWindow(CWnd * pParentWnd);
    void GetActiveArea(LPRECT lpRect) CONST;
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    DECLARE_MESSAGE_MAP()
    afx_msg void OnClose();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

    // Menu items

    afx_msg void OnDrawwindow100();
    afx_msg void OnDrawwindowNext();
    afx_msg void OnDrawwindowPrev();
    afx_msg void OnDrawwindowZoomin();
    afx_msg void OnDrawwindowZoomout();
    afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
};
