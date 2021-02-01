#include "pch.h"
#include "HFHeaderDropDownList.h"

HFHeaderDropDownList::HFHeaderDropDownList() :CComboBox() {}
HFHeaderDropDownList::~HFHeaderDropDownList() {}

BOOL HFHeaderDropDownList::CreateHFFHeaderDropDownList(CRect CONST& rect, CWnd* pParentWnd, UINT nID) {
    return Create(WS_VISIBLE | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_SORT, rect, pParentWnd, nID);
}

BEGIN_MESSAGE_MAP(HFHeaderDropDownList, CComboBox)
    ON_WM_CREATE()
END_MESSAGE_MAP()

// HFHeaderDropDownList message handlers

int HFHeaderDropDownList::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CComboBox::OnCreate(lpCreateStruct) == -1)
        return -1;

    for (int i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        AddString(HFLC::header::CODE_TO_LPTSTR[i]);
    }

    return 0;
}
