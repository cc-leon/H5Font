#pragma once
#include "HFLogWindow.h"
#include "HFDrawWindow.h"

class HFMainWindow : public CFrameWnd {
protected:
    HFLogWindow * m_logWnd;
    HFDrawWindow* m_drawWnd;
    CMenu m_mnMain;
    CButton m_btnMain;

    BOOL GetMenuChecked(UINT uiMenuID);
    VOID SetMenuWindowsLogChecked(BOOL bChecked);
    VOID SetMenuWindowsDrawChecked(BOOL bChecked);

public:
    HFMainWindow();
    virtual ~HFMainWindow();
    CWnd * GetLogWnd();
    BOOL CreateHFMainWindow();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    DECLARE_MESSAGE_MAP()
    afx_msg void OnClose();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnWindowlog(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnWindowdraw(WPARAM wParam, LPARAM lParam);

    // Menus
    afx_msg void OnStartSetup();
    afx_msg void OnStartRun();
    afx_msg void OnStartExit();
    afx_msg void OnWindowsLog();
    afx_msg void OnWindowsDraw();
    afx_msg void OnHelpReadme();
    afx_msg void OnHelpOnline();
    afx_msg void OnHelpAbout();
    afx_msg void OnLanguageEnglish();
    afx_msg void OnLanguageChinese();
    afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
    afx_msg void ONMainBtnClicked();
};
