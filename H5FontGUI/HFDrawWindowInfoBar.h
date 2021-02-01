#pragma once
class HFDrawWindowInfoBar : public CStatusBarCtrl {
public:
    HFDrawWindowInfoBar();
    virtual ~HFDrawWindowInfoBar();

    BOOL CreateHFDrawWindowInfoBar(CWnd* pParentWnd, UINT nID = HFUIC::DrawWindow::ID_infoBar);
    VOID UpdateFromParent();
    VOID SetHFDrawWindowInforBarText(CString CONST& sPaneText, int iPaneNumber);
    VOID SetHFDrawWindowInforBarText(CString CONST& sSimplePaneText);

    DECLARE_MESSAGE_MAP()
};
