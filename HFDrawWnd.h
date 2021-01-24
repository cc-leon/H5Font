#pragma once


// HFDrawWnd frame

class HFDrawWnd : public CFrameWnd {
    DECLARE_DYNCREATE(HFDrawWnd)
protected:
    HFDrawWnd();           // protected constructor used by dynamic creation
    virtual ~HFDrawWnd();

protected:
    DECLARE_MESSAGE_MAP()
};
