#pragma once
#include "HFMemDC.h"

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
    BOOL DrawAtOnce(LPCFONTINFO fiInfo);
    BOOL DrawOneByOne(LPCFONTINFO fiInfo, int& i);

    BOOL SaveAtOnce();
    BOOL SaveOneByOne(int& i);

protected:
    HFMemDC m_dcs[HFLC::header::HEADER_COUNT];
};

extern HFDrawDCsCentre hfcore;