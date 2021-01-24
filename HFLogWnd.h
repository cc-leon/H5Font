#pragma once


// HFLogWnd frame

class HFLogWnd : public CFrameWnd {
    DECLARE_DYNCREATE(HFLogWnd)
protected:
    HFLogWnd();           // protected constructor used by dynamic creation
    virtual ~HFLogWnd();

protected:
    DECLARE_MESSAGE_MAP()
};
