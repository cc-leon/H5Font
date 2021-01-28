#include "pch.h"
#include "HFRichTextBox.h"


HFRichTextBox::HFRichTextBox(): CRichEditCtrl(), m_lMaxHLen(0) {}

HFRichTextBox::~HFRichTextBox() {}

BEGIN_MESSAGE_MAP(HFRichTextBox, CRichEditCtrl)
    ON_CONTROL_REFLECT(EN_CHANGE, &HFRichTextBox::OnEnChange)
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_CREATE()
END_MESSAGE_MAP()

void HFRichTextBox::OnEnChange() {
/*
    SCROLLINFO info = { 0 };
    CRect aa;
    GetRect(aa);
    info.fMask = SIF_ALL;
    info.nMin = 0;
    info.nMax = GetLineCount();
    info.nPos = info.nMax;
    info.nPage = 20;
    info.cbSize = sizeof(SCROLLINFO);
    this->SetScrollInfo(SB_VERT, &info);

    if (GetLineCount() <= 0) {
        m_lMaxHLen = 0;
    }
    else {
        int i = GetLineCount();
        int j = LineIndex(i - 1);
        int k = LineLength(j - 1);
        if (LineLength(LineIndex(GetLineCount() -1) -1) > m_lMaxHLen) {
            m_lMaxHLen = LineLength(LineIndex(GetLineCount() - 1) - 1);
        }
    }
    info.nMax = 400;
    info.nPage = 1;
    long startc, endc;
    GetSel(startc, endc);
    info.nPos = 0;
    this->SetScrollInfo(SB_HORZ, &info);
    */
    long startc, endc;
    GetSel(startc, endc);
}


void HFRichTextBox::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
    CRichEditCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}


void HFRichTextBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
    CRichEditCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}


int HFRichTextBox::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    //SetEventMask(GetEventMask() | ENM_CHANGE | ENM_REQUESTRESIZE);
    return 0;
}
