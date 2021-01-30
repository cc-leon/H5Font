#include "pch.h"
#include "HFImageDisplay.h"

HFImageDisplay::HFImageDisplay() : CStatic(), m_pMemDC(NULL), m_dwZoomValue(100) {}

HFImageDisplay::~HFImageDisplay() {}

BOOL HFImageDisplay::CreateHFImageDisplay(CPoint CONST& ptTopLeft, CWnd* pParentWnd, HFMemDC* pMemDC, UINT nID) {
    CRect rect;
    rect.left = ptTopLeft.x;
    rect.top = ptTopLeft.y;
    if (pMemDC != NULL) {
        CSize csMemDC = pMemDC->GetMemDCCSize();
        rect.right = csMemDC.cx;
        rect.bottom = csMemDC.cy;
    }
    else {
        rect.right = 0;
        rect.bottom = 0;
    }

    m_pMemDC = pMemDC;
    BOOL bResult = CStatic::Create(NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_REALSIZECONTROL, rect, pParentWnd, nID);
    return bResult;
}

DWORD HFImageDisplay::GetZoom() CONST {
    return m_dwZoomValue;
}

BOOL HFImageDisplay::SetZoom(DWORD dwScale, CRect CONST& rcParentClient, CPoint CONST& ptCursor) {
    if (dwScale >= 10 && dwScale <= 400 && m_pMemDC != NULL) {
        m_dwZoomValue = dwScale;
        CRect rcWnd = ui::GetWindowCRectInParent(this);

        CSize csMemDC = m_pMemDC->GetMemDCCSize();
        CSize csNew(csMemDC.cx * dwScale / 100, csMemDC.cy * dwScale / 100);
        rcWnd = ui::ZoomCRectOverCPoint(csNew, rcWnd, ptCursor);

        CRect rcParent = rcParentClient;
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
        RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
        return TRUE;
    }
    else {
        return FALSE;
    }
}

CPoint HFImageDisplay::DisplayToLogical(CPoint CONST& ptControl) {
    return CPoint (ptControl.x * 100 / m_dwZoomValue, ptControl.y * 100 / m_dwZoomValue);
}

WCHAR HFImageDisplay::QueryCharUnicode(CPoint CONST& ptControl) {
    return 0;
}

CRect HFImageDisplay::QueryCharCRect(CPoint CONST& ptControl) {
    return CRect();
}

VOID HFImageDisplay::QueryCharABC(CPoint CONST& ptControl, int* aiABC) {
    return;
}

BEGIN_MESSAGE_MAP(HFImageDisplay, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void HFImageDisplay::OnPaint() {
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CStatic::OnPaint() for painting messages
    if (m_pMemDC != NULL) {
        /*
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
        dc.StretchBlt(0, 0, dispRect.Width(), dispRect.Height(), &memdc, 0, 0, bmpSize.cx, bmpSize.cy, SRCCOPY);*/
    }
}
