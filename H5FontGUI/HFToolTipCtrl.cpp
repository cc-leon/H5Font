#include "pch.h"
#include "HFToolTipCtrl.h"

HFToolTipCtrl::HFToolTipCtrl() : CToolTipCtrl() {}

HFToolTipCtrl::~HFToolTipCtrl() { }

VOID HFToolTipCtrl::AddToolThatWorks(LPTSTR szTips, CWnd* pWnd, LPCRECT lpRectTool) {
    TOOLINFO toolinfo;

    toolinfo.cbSize = sizeof(TOOLINFO);
    toolinfo.lpszText = szTips;
    toolinfo.hwnd = pWnd->GetParent()->GetSafeHwnd();
    toolinfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    toolinfo.uId = (UINT_PTR)pWnd->GetSafeHwnd();
    if (lpRectTool != NULL) {
        toolinfo.rect = *lpRectTool;
    }

    this->SendMessage(TTM_ADDTOOL, 0, (LPARAM)&toolinfo);
};

VOID HFToolTipCtrl::ChangeTips(LPTSTR szNewTips, CWnd* pWnd) {
    CToolInfo toolinfo;
    GetToolInfo(toolinfo, pWnd);
    toolinfo.lpszText = szNewTips;
    SetToolInfo(&toolinfo);
}


BEGIN_MESSAGE_MAP(HFToolTipCtrl, CToolTipCtrl)
END_MESSAGE_MAP()

// HFToolTipCtrl message handlers

/*
* 
*/