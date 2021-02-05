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

VOID HFImageDisplay::LoadHFMemDC(HFMemDC* pMemDC) {
    m_pMemDC = pMemDC;
    if (pMemDC != NULL) {
        CRect rect;
        CSize csMemDC = pMemDC->GetMemDCCSize();
        rect.right = csMemDC.cx;
        rect.bottom = csMemDC.cy;
        SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), NULL);
    }
    RedrawWindow();
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
                       // Do not call CStatic::OnPaint() for painting messages
    if (m_pMemDC != NULL) {
        CRect dispRect;
        GetClientRect(dispRect);
        CSize memDCSize = m_pMemDC->GetMemDCCSize();
        dc.SetStretchBltMode(HALFTONE);
        dc.StretchBlt(
            0, 0, dispRect.Width(), dispRect.Height(), 
            m_pMemDC, 0, 0, memDCSize.cx, memDCSize.cy, SRCCOPY);
    }
}
