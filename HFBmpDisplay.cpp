#include "pch.h"
#include "HFBmpDisplay.h"

HFBmpDisplay::HFBmpDisplay() : CStatic(), m_bmpSource(NULL), m_dwZoomValue(100) {}

HFBmpDisplay::~HFBmpDisplay() {}

HFBitmap* HFBmpDisplay::SetHFBitmap(HFBitmap* bmpNew) {
    HFBitmap * bmpPrev = m_bmpSource;
    m_bmpSource = bmpNew;
    SetBitmap(*m_bmpSource);
    return bmpPrev;
}

DWORD HFBmpDisplay::GetZoom() CONST {
    return m_dwZoomValue;
}

BOOL HFBmpDisplay::SetZoom(DWORD dwScale, CPoint CONST& ptCursor) {
    if (dwScale >= 10 && dwScale <= 400) {
        m_dwZoomValue = dwScale;
        BITMAP bmpInfo;
        m_bmpSource->GetBitmap(&bmpInfo);
        CRect rcWnd = ui::GetWindowCRectInParent(this);
        CSize csNew(bmpInfo.bmWidth * dwScale / 100, bmpInfo.bmHeight * dwScale / 100);
        rcWnd = ui::ZoomCRectOverCPoint(csNew, rcWnd, ptCursor);
        CRect rcParent;
        ::GetClientRect(*this->GetParent(), rcParent);
        rcParent.MoveToXY(0, 0);
        LONG lLeftOut = rcParent.left - rcWnd.left;
        LONG lTopOut = rcParent.top - rcWnd.top;
        LONG lRightOut = rcWnd.right - rcParent.right;
        LONG lBottomOut = rcWnd.bottom - rcParent.bottom;
        if (lLeftOut > 0 && lRightOut < 0 ) {
            rcWnd.OffsetRect(min(-lRightOut, lLeftOut), 0);
        }
        else if (lLeftOut < 0 && lRightOut > 0) {
            rcWnd.OffsetRect(max(-lRightOut, lLeftOut), 0);
        }

        if (lTopOut > 0 && lBottomOut < 0 ) {
            rcWnd.OffsetRect(0, min(-lBottomOut, lTopOut));
        }
        else if (lTopOut < 0 && lBottomOut > 0) {
            rcWnd.OffsetRect(0, max(-lBottomOut, lTopOut));
        }

        SetWindowPos(&wndTop, rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), NULL);
        RedrawWindow();
        return TRUE;
    }
    else {
        return FALSE;
    }
}

BEGIN_MESSAGE_MAP(HFBmpDisplay, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL HFBmpDisplay::Create(CPoint CONST& ptTopLeft, HFBitmap* bmpNew, CWnd* pParentWnd, UINT nID) {
    CRect rect;
    rect.left = ptTopLeft.x;
    rect.top = ptTopLeft.y;
    CSize bmpSize = bmpNew->GetBmpCSize();
    rect.right = rect.left + bmpSize.cx;
    rect.bottom = rect.top + bmpSize.cy;
    BOOL bResult = CStatic::Create(NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_REALSIZECONTROL, rect, pParentWnd, nID);
    SetHFBitmap(bmpNew);
    return bResult;
}


void HFBmpDisplay::OnPaint() {
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CStatic::OnPaint() for painting messages
    if (m_bmpSource != NULL) {
        CDC memdc;
        memdc.CreateCompatibleDC(NULL);
        CBitmap freshMap;
        freshMap.CreateBitmap(1000, 1000,1, 32, NULL);
        CBitmap * oldmap = memdc.SelectObject(m_bmpSource);
        memdc.SetBkMode(TRANSPARENT);
        memdc.SetTextColor(UIConst::Color::WHITE);
        CFont newFont;
        newFont.CreateFont(
            30,                            // Height
            0,                             // Width
            0,                             // Escapement
            0,                             // OrientationME
            FW_BOLD,                       // Weight
            FALSE,                         // Italic
            FALSE,                          // Underline
            0,                             // StrikeOut
            ANSI_CHARSET,                  // CharSet
            OUT_DEVICE_PRECIS,            // OutPrecision
            CLIP_DEFAULT_PRECIS,           // ClipPrecision
            CLEARTYPE_QUALITY,               // Quality
            FIXED_PITCH | FF_ROMAN,      // PitchAndFamily
            _T("Monotype Corsiva"));      // Facename
        memdc.SelectObject(newFont);
        CString aa(_T("Heroes of might and magic V"));
        memdc.ExtTextOut(0, 0, ETO_CLIPPED, NULL, aa, NULL);

        CRect dispRect;
        GetClientRect(dispRect);
        CSize bmpSize = m_bmpSource->GetBmpCSize();
        dc.SetStretchBltMode(HALFTONE);
        dc.StretchBlt(0, 0, dispRect.Width(), dispRect.Height(), &memdc, 0, 0, bmpSize.cx, bmpSize.cy, SRCCOPY);
    }
}


BOOL HFBmpDisplay::OnEraseBkgnd(CDC* pDC) {

    return CStatic::OnEraseBkgnd(pDC);
}
