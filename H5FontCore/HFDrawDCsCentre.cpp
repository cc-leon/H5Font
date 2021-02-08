#include "pch.h"
#include "HFDrawDCsCentre.h"

HFDrawDCsCentre hfcore;

HFDrawDCsCentre::HFDrawDCsCentre() {}
HFDrawDCsCentre::~HFDrawDCsCentre() {}

HFMemDC& HFDrawDCsCentre::operator [] (size_t index) {
    return m_dcs[index];
}

VOID HFDrawDCsCentre::DrawAtOnce(LPCFONTINFO fiInfo) {
    for (size_t i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        if (i != HFLC::header::HEADER_32) continue;
        CString sTemp;
        m_dcs[i].CreateHFMemDC(fiInfo[i], i);
        sTemp.Format(_T("D:\\data\\%s.png"), HFLC::header::CODE_TO_LPTSTR[i]);
        m_dcs[i].SaveDDS(sTemp);
    }
}