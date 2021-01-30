#pragma once
#include "HFRichTextBox.h"

class HFLogWindow : public CFrameWnd {
protected:
    HFRichTextBox* m_txtLog;
    static int const SS_MSG_HIDE_LOGWND = WM_APP + 4;

public:
    HFLogWindow();
    virtual ~HFLogWindow();

    BOOL CreateHFLogWindow(CWnd * pParentWnd);

    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    DECLARE_MESSAGE_MAP()
    afx_msg void OnClose();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
