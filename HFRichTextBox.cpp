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
    SCROLLINFO info = { 0 };
    info.fMask = SIF_ALL;
    info.nMin = 0;
    info.nMax = GetLineCount();
    info.nPos = LineFromChar(-1);
    info.nPage = 1;
    info.cbSize = sizeof(SCROLLINFO);
    this->SetScrollInfo(SB_VERT, &info);

    if (GetLineCount() <= 1) {
        m_lMaxHLen = 0;
    }
    if (LineLength(GetLineCount() - 1) > m_lMaxHLen) {
        m_lMaxHLen = LineLength(GetLineCount() - 1);
    }
    info.nMax = m_lMaxHLen;
    info.nPage = 10;
    long startc, endc;
    GetSel(startc, endc);
    info.nPos = endc;
    this->SetScrollInfo(SB_HORZ, &info);
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

    SetEventMask(GetEventMask() | ENM_CHANGE);
    return 0;
}
