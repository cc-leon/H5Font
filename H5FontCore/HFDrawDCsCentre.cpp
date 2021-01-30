#include "pch.h"
#include "HFDrawDCsCentre.h"

HFDrawDCsCentre hfcore;

HFDrawDCsCentre::HFDrawDCsCentre() {}
HFDrawDCsCentre::~HFDrawDCsCentre() {}

HFMemDC& HFDrawDCsCentre::operator [] (size_t index) {
    return m_dcs[index];
}
