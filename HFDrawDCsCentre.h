#pragma once
#include "HFBitmap.h"
#include "HFMemDC.h"

#define DRAW_CENTRE HFDrawDCsCentre::get()

class HFDrawDCsCentre {
public:
    enum {
        BUTTON_16 = 0,
        DEFAULT_18, DEFAULT_20, DEFAULT_22, DEFAULT_26, DEFAULT_32, DEFAULT_40,
        HEADER_22, HEADER_26, HEADER_32, SYSTEM_10, SYSTEM_16, SYSTEM_24,
        INDEX_COUNT
    };

    HFDrawDCsCentre(HFDrawDCsCentre CONST&) = delete;
    HFDrawDCsCentre(HFDrawDCsCentre&&) = delete;
    VOID operator = (HFDrawDCsCentre CONST&) = delete;
    VOID operator = (HFDrawDCsCentre&&) = delete;

    HFMemDC& operator [] (size_t index);
    HFDrawDCsCentre& get();
    virtual ~HFDrawDCsCentre();

protected:
    HFDrawDCsCentre();
    HFMemDC m_dcs[INDEX_COUNT];
};
