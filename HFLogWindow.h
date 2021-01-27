#pragma once
#include "HFRichTextBox.h"

class HFLogWindow : public CFrameWnd {
protected:
    enum {
        ID_txtLog = UIConst::LogWindow::ID + 1,
    };

    HFRichTextBox* m_txtLog;
    CWnd* m_parentWnd;
    static int const SS_MSG_HIDE_LOGWND = WM_APP + 4;

public:
    HFLogWindow();
    virtual ~HFLogWindow();

    DECLARE_MESSAGE_MAP()

    afx_msg void OnClose();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    afx_msg LRESULT OnLogmsg(WPARAM wParam, LPARAM lParam);
};
