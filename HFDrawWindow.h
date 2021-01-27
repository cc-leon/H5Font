#pragma once

class HFDrawWindow : public CFrameWnd {
protected:
    
public:
    HFDrawWindow();           // protected constructor used by dynamic creation
    virtual ~HFDrawWindow();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};
