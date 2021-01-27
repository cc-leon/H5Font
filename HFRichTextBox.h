#pragma once

class HFRichTextBox : public CRichEditCtrl {
protected:
    LONG m_lMaxHLen;
public:
    HFRichTextBox();
    virtual ~HFRichTextBox();

    DECLARE_MESSAGE_MAP()
    afx_msg void OnEnChange();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
