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
        m_dcs[i].CreateHFMemDC(fiInfo[i]);
        break;
    }
}