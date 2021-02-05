#pragma once
class HFBitMask {
public:
    HFBitMask();
    HFBitMask(HFBitMask CONST& other);
    HFBitMask(HFBitMask && other);
    ~HFBitMask();
    HFBitMask& operator=(HFBitMask CONST& other);
    HFBitMask& operator=(HFBitMask && other);
    BOOL Initiate(size_t cbmSize);
    BOOL GetBit(int Index);
    BOOL SetBit(int Index);

protected:
    LPDWORD m_lpdwMasks;
};

