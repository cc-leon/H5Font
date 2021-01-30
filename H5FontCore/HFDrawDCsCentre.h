#pragma once
#include "HFBitmap.h"
#include "HFMemDC.h"
#include "HFFont.h"

#define DRAW_CENTRE HFDrawDCsCentre::get()

class HFDrawDCsCentre {
public:
    HFDrawDCsCentre();
    virtual ~HFDrawDCsCentre();
    HFDrawDCsCentre(HFDrawDCsCentre CONST&) = delete;
    HFDrawDCsCentre(HFDrawDCsCentre&&) = delete;
    VOID operator = (HFDrawDCsCentre CONST&) = delete;
    VOID operator = (HFDrawDCsCentre&&) = delete;

    HFMemDC& operator [] (size_t index);
    VOID InitializeInstance(LPTSTR szTargetFolder);

protected:
    HFMemDC m_dcs[LogicConst::INDEX_COUNT];
};

extern HFDrawDCsCentre hfcore;