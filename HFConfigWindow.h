#pragma once
#include "HFLogWindow.h"
#include "HFDrawWindow.h"

class HFConfigWindow : public CFrameWnd {
protected:
    HFLogWindow * m_logWnd;
    HFDrawWindow* m_drawWnd;

public:
    HFConfigWindow();
    virtual ~HFConfigWindow();
    CWnd * GetLogWnd();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    DECLARE_MESSAGE_MAP()
    afx_msg void OnClose();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
