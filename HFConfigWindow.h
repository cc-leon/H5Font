#pragma once

class HFConfigWindow : public CFrameWnd {
protected:
    CFrameWnd * m_logWnd;
    CFrameWnd* m_drawWnd;

public:
    HFConfigWindow();
    virtual ~HFConfigWindow();
    VOID SetLogWnd(CFrameWnd * CONST pLogWnd);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
