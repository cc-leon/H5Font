#pragma once

class HFRichTextBox : public CRichEditCtrl {
public:
    HFRichTextBox();
    virtual ~HFRichTextBox();
    BOOL CreateHFRichTextBox(CWnd* pParentWnd, CRect CONST& rect, UINT nID=HFUIC::LogWindow::ID_txtLog);

    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnEnChange();
    afx_msg LRESULT OnLogmsg(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnLogsave(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnLogclear(WPARAM wParam, LPARAM lParam);
};
