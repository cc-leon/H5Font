#pragma once

class HFBitMask {
protected:
    class HFBitRef {
    public:
        HFBitRef() = delete;
        HFBitRef(BOOL bValue, SIZE_T uiIndex, HFBitMask * pbmParent);
        HFBitRef(HFBitRef CONST& other) = default;
        HFBitRef(HFBitRef && other) = default;
        HFBitRef& operator = (BOOL other);
        HFBitRef& operator = (bool other);
        operator BOOL() CONST;
        operator bool() CONST;
        HFBitRef& operator = (HFBitRef CONST& other) = default;
        HFBitRef& operator = (HFBitRef && other) = default;

    private:
        BOOL m_bValue;
        SIZE_T m_uiIndex;
        HFBitMask * m_pbmParent;
    };

public:
    HFBitMask();
    virtual ~HFBitMask();
    explicit HFBitMask(SIZE_T uiSize);
    HFBitMask(HFBitMask CONST& other);
    HFBitMask(HFBitMask && other) noexcept;
    HFBitMask& operator =(HFBitMask CONST& other);
    HFBitMask& operator =(HFBitMask && other) noexcept;
    HFBitRef operator [](SIZE_T uiIndex);
    BOOL GetBit(SIZE_T uiIndex) CONST;
    BOOL SetBit(SIZE_T uiIndex, BOOL bValue);
    bool SetBit(SIZE_T uiIndex, bool fValue);
    VOID CreateBitMask(SIZE_T uiSize);
    SIZE_T GetBitsCount() CONST;

protected:
    LPDWORD m_lpdwData;
    SIZE_T m_cdwSize;
};

