#include "pch.h"
#include "HFFontDropDownList.h"

int CALLBACK EnumFontFamProc(ENUMLOGFONT* lpelf, NEWTEXTMETRIC* lpntm, DWORD FontType, LPARAM lParam) {
    HFFontDropDownList* ddlFont = (HFFontDropDownList*)lParam;
    if ((FontType & TRUETYPE_FONTTYPE) | (FontType & DEVICE_FONTTYPE)) {
        if (lpelf->elfLogFont.lfFaceName[0] != _T('@')) {
            if (ddlFont->FindStringExact(0, lpelf->elfLogFont.lfFaceName)) {
                ddlFont->AddString(lpelf->elfLogFont.lfFaceName);
            }
        }
    }
    return 1;
}

HFFontDropDownList::HFFontDropDownList() : CComboBox() {}

HFFontDropDownList::~HFFontDropDownList() {}

BOOL HFFontDropDownList::CreateHFFontDropDownList(CRect CONST& rect, CWnd* pParentWnd, UINT nID) {
    return Create(WS_VISIBLE | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_SORT, rect, pParentWnd, nID);
}

size_t HFFontDropDownList::GetAllFontFaces() {
    HDC hdc = ::CreateCompatibleDC(NULL);
    ::EnumFontFamilies(hdc, NULL, (FONTENUMPROC)&EnumFontFamProc, (LPARAM)this);
    ::DeleteDC(hdc);
    CString sTemp;
    sTemp.Format(HFSTRC(IDS_LOG_NUM_FONTS_LOADED), GetCount());
    LOG.log(sTemp, HFUIC::Color::GREEN);
    return 0;
}

BEGIN_MESSAGE_MAP(HFFontDropDownList, CComboBox)
    ON_WM_CREATE()
END_MESSAGE_MAP()

// HFFontDropDownList message handlers

int HFFontDropDownList::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CComboBox::OnCreate(lpCreateStruct) == -1)
        return -1;

    GetAllFontFaces();
    return 0;
}
