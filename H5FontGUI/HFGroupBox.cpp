#include "pch.h"
#include "HFGroupBox.h"

HFGroupBox::HFGroupBox() {}

HFGroupBox::~HFGroupBox() {}

BOOL HFGroupBox::CreateHFGroupBox(LPCTSTR szText, CWnd* pParent, CRect CONST& rect) {
    return Create(szText, WS_CHILD | WS_VISIBLE | BS_GROUPBOX, rect, pParent, NULL);
}

BEGIN_MESSAGE_MAP(HFGroupBox, CButton)
END_MESSAGE_MAP()

// HFGroupBox message handlers

BOOL HFGroupBox::OnCommand(WPARAM wParam, LPARAM lParam) {
    GetParent()->SendMessage(WM_COMMAND, wParam, lParam);
    return CButton::OnCommand(wParam, lParam);
}
