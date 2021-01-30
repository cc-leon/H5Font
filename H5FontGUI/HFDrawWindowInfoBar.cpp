// HFDrawWindowInfoBar.cpp : implementation file
//

#include "pch.h"
#include "HFDrawWindowInfoBar.h"


// HFDrawWindowInfoBar


HFDrawWindowInfoBar::HFDrawWindowInfoBar() {}

HFDrawWindowInfoBar::~HFDrawWindowInfoBar() {}

BOOL HFDrawWindowInfoBar::CreateHFDrawWindowInfoBar(CWnd* pParentWnd, UINT nID) {
    BOOL bResult = Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP, CRect(), pParentWnd, nID);
    UpdateFromParent();
    return bResult;
}

VOID HFDrawWindowInfoBar::UpdateFromParent() {
    SetWindowPos(&wndBottom, 0, 0, 0, 0, NULL);

    CRect rcParent;
    GetParent()->GetWindowRect(rcParent);

    int aiWidths[] = {
        rcParent.Width() * UIConst::DrawWindow::InfoBar::PANE1_PERCENTAGE /100,
        rcParent.Width() * UIConst::DrawWindow::InfoBar::PANE2_PERCENTAGE /100,
        rcParent.Width() * UIConst::DrawWindow::InfoBar::PANE3_PERCENTAGE /100, -1 };

    for (int i = 1; i < 3; i++) {
        aiWidths[i] += aiWidths[i - 1];
    }

    SetParts(4, aiWidths);
}

VOID HFDrawWindowInfoBar::SetHFDrawWindowInforBarText(CString CONST& sPaneText, int iPaneNumber) {
    if (IsSimple()) {
        SetSimple(FALSE);
    }
    SetText(CString(_T(" ")) + sPaneText, iPaneNumber, NULL);
}

VOID HFDrawWindowInfoBar::SetHFDrawWindowInforBarText(CString CONST& sSimplePaneText) {
    if (!IsSimple()) {
        SetSimple(TRUE);
    }
    SetText(CString(_T("    ")) + sSimplePaneText, 255, SBT_NOBORDERS);
}

BEGIN_MESSAGE_MAP(HFDrawWindowInfoBar, CStatusBarCtrl)
END_MESSAGE_MAP()

// HFDrawWindowInfoBar message handlers
