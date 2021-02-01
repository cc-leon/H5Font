#pragma once

class HFFontDropDownList : public CComboBox {
public:
    HFFontDropDownList();
    virtual ~HFFontDropDownList();
    BOOL CreateHFFontDropDownList(CRect CONST& rect, CWnd* pParentWnd, UINT nID);

protected:
    size_t GetAllFontFaces();

    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
