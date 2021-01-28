#include "pch.h"
#include "HFDrawDCsCentre.h"

HFDrawDCsCentre::HFDrawDCsCentre() {}
HFDrawDCsCentre::~HFDrawDCsCentre() {}

HFMemDC& HFDrawDCsCentre::operator [] (size_t index) {
    return m_dcs[index];
}

HFDrawDCsCentre& HFDrawDCsCentre::get() {
    static HFDrawDCsCentre onlyInstance;
    return onlyInstance;
}
