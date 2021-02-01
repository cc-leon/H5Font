#pragma once

class HFHeaderDropDownList : public CComboBox {
public:
    HFHeaderDropDownList();
    virtual ~HFHeaderDropDownList();

    BOOL CreateHFFHeaderDropDownList(CRect CONST& rect, CWnd* pParentWnd, UINT nID);

protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
