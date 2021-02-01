#pragma once

class HFGroupBox : public CButton {
public:
    HFGroupBox();
    virtual ~HFGroupBox();
    BOOL CreateHFGroupBox(LPCTSTR szText, CWnd* pParent, CRect CONST& rect);

protected:
    DECLARE_MESSAGE_MAP()
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
