#pragma once

class HFToolTipCtrl : public CToolTipCtrl {
public:
    HFToolTipCtrl();
    virtual ~HFToolTipCtrl();

    //pWnd is the control that needs tips displayed
    VOID AddToolThatWorks(LPTSTR szTips, CWnd* pWnd, LPCRECT lpRectTool = NULL);

    //pWnd is the control that needs tips displayed
    VOID ChangeTips(LPTSTR szNewTips, CWnd* pWnd);

protected:
    DECLARE_MESSAGE_MAP()
};
