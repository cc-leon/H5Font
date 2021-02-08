#include "pch.h"
#include "HFBitMask.h"

HFBitMask::HFBitRef::HFBitRef(BOOL bValue, SIZE_T uiIndex, HFBitMask * pbmParent)
    : m_bValue(bValue), m_uiIndex(uiIndex), m_pbmParent(pbmParent) {}

HFBitMask::HFBitRef& HFBitMask::HFBitRef::operator = (BOOL other) {
    m_pbmParent->SetBit(m_uiIndex, other);
    return *this;
}

HFBitMask::HFBitRef& HFBitMask::HFBitRef::operator = (bool other) {
    m_pbmParent->SetBit(m_uiIndex, other);
    return *this;
}

HFBitMask::HFBitRef::operator BOOL() CONST{
    return m_bValue;
}

HFBitMask::HFBitRef::operator bool()  CONST{
    return (bool)m_bValue;
}


HFBitMask::HFBitMask() : m_lpdwData(NULL), m_cdwSize(0) {}

HFBitMask::~HFBitMask() {
    mem::FreeMem(m_lpdwData);
}

HFBitMask::HFBitMask(SIZE_T uiSize) : m_lpdwData(NULL), m_cdwSize(0) {
    CreateBitMask(uiSize);
}

HFBitMask::HFBitMask(HFBitMask CONST& other) : m_cdwSize(other.m_cdwSize) {
    m_lpdwData = mem::GetMem<DWORD>(m_cdwSize * sizeof(DWORD));
    ::CopyMemory(m_lpdwData, other.m_lpdwData, other.m_cdwSize * sizeof(DWORD));
}

HFBitMask::HFBitMask(HFBitMask&& other) noexcept: m_lpdwData(other.m_lpdwData), m_cdwSize(other.m_cdwSize) {
}

HFBitMask& HFBitMask::operator =(HFBitMask CONST& other) {
    m_cdwSize = other.m_cdwSize;
    m_lpdwData = mem::GetMem<DWORD>(m_cdwSize * sizeof(DWORD));
    ::CopyMemory(m_lpdwData, other.m_lpdwData, other.m_cdwSize * sizeof(DWORD));
    return *this;
}

HFBitMask& HFBitMask::operator =(HFBitMask&& other) noexcept {
    m_lpdwData = other.m_lpdwData;
    m_cdwSize = other.m_cdwSize;
    return *this;
}

HFBitMask::HFBitRef HFBitMask::operator [](SIZE_T uiIndex) {
    return HFBitRef(GetBit(uiIndex), uiIndex, this);
}

BOOL HFBitMask::GetBit(SIZE_T uiIndex) CONST {
    return (m_lpdwData[uiIndex / sizeof(DWORD)] & (1 << uiIndex % sizeof(DWORD))) > 0;
}

BOOL HFBitMask::SetBit(SIZE_T uiIndex, BOOL bValue) {
    if (bValue) {
        m_lpdwData[uiIndex / sizeof(DWORD)] |= (1 << uiIndex % sizeof(DWORD));
    }
    else {
        if (GetBit(uiIndex)) {
            m_lpdwData[uiIndex / sizeof(DWORD)] &= ~(1 << uiIndex % sizeof(DWORD));
        }
    }
    return bValue;
}

bool HFBitMask::SetBit(SIZE_T uiIndex, bool fValue) {
    return (bool)SetBit(uiIndex, BOOL(fValue));
}


VOID HFBitMask::CreateBitMask(SIZE_T uiSize) {
    m_cdwSize = uiSize / sizeof(DWORD);
    if (m_cdwSize % sizeof(DWORD)) {
        m_cdwSize += 1;
    }

    mem::FreeMem(m_lpdwData);
    m_lpdwData = mem::GetMem<DWORD>(m_cdwSize);
    ::ZeroMemory(m_lpdwData, sizeof(DWORD) * m_cdwSize);
}

SIZE_T HFBitMask::GetBitsCount() CONST {
    return m_cdwSize * sizeof(DWORD);
}