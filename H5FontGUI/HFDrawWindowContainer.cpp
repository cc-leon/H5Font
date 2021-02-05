// HFDrawWindowContainer.cpp : implementation file
//

#include "pch.h"
#include "HFDrawWindowContainer.h"
#include "HFDrawWindow.h"

// HFDrawWindowContainer

HFDrawWindowContainer::HFDrawWindowContainer() 
    : CWnd(), m_dispImage(NULL), m_infobar(NULL), m_ptPrevCursor(NULL), m_hPrevCursor(NULL) {
    RegisterThisClass();
}

HFDrawWindowContainer::~HFDrawWindowContainer() {}

BOOL HFDrawWindowContainer::RegisterThisClass() {
    CString sWarning;

    WNDCLASS wndclass;
    ::ZeroMemory((LPVOID*)(&wndclass), sizeof(WNDCLASS));
    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!::GetClassInfo(hInst, HFUIC::DrawWindow::Container::szDrawWindowContainerName, &wndclass)) {
        wndclass.lpfnWndProc = AfxWndProc;
        //wndclass.cbClsExtra = 0;
        //wndclass.cbWndExtra = 0;
        wndclass.hInstance = hInst;
        wndclass.hIcon = NULL;
        wndclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        wndclass.lpszMenuName = NULL;
        wndclass.lpszClassName = HFUIC::DrawWindow::Container::szDrawWindowContainerName;
        if (!AfxRegisterClass(&wndclass)) {
            sWarning.Format(_T("\"%s\" class is not registered successfully"), HFUIC::DrawWindow::Container::szDrawWindowContainerName);
            ::AfxMessageBox(sWarning);
            return FALSE;
        }
    }
    else {
        sWarning.Format(_T("\"%s\" class is already registered"), HFUIC::DrawWindow::Container::szDrawWindowContainerName);
        ::AfxMessageBox(sWarning);
        return FALSE;
    }
    return TRUE;
}

BOOL HFDrawWindowContainer::CreateHFDrawWindowContainer(CWnd* pParentWnd, HFDrawWindowInfoBar* pInfoBar, UINT nID) {
    HFDrawWindow* pParentHFWnd = dynamic_cast<HFDrawWindow*> (pParentWnd);
    CRect rect;
    pParentHFWnd->GetActiveArea(rect);
    BOOL bResult = Create(
        _T("HFDrawWindowContainer"), NULL,
        WS_CHILD | WS_BORDER | WS_VISIBLE | WS_GROUP | WS_TABSTOP,
        rect,
        pParentWnd, nID);
    m_infobar = pInfoBar;
    return bResult;
}

VOID HFDrawWindowContainer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
    ScreenToClient(&pt);

    CRect rcImage = ui::GetWindowCRectInParent(m_dispImage);
    CRect rcParentRect;
    GetClientRect(rcParentRect);
    if (rcImage.PtInRect(pt) && zDelta != 0) {
        CRect rcImage = ui::GetWindowCRectInParent(m_dispImage);
        m_dispImage->SetZoom(
            m_dispImage->GetZoom() + zDelta / WHEEL_DELTA * 10,
            rcParentRect,
            pt - rcImage.TopLeft());
    }
}

VOID HFDrawWindowContainer::RestoreZoom() {
    CRect rcParentRect;
    GetClientRect(rcParentRect);
    m_dispImage->SetZoom(100, rcParentRect, CPoint(0, 0));
    m_dispImage->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE);
}

CPoint HFDrawWindowContainer::GetImageCenter() {
    CRect rcImage;
    m_dispImage->GetWindowRect(rcImage);
    return rcImage.CenterPoint();;
}

VOID HFDrawWindowContainer::SetMemDC(HFMemDC* memDC) {
    m_dispImage->LoadHFMemDC(memDC);
    RestoreZoom();
}

BOOL HFDrawWindowContainer::PreCreateWindow(CREATESTRUCT& cs) {
    if (!CWnd::PreCreateWindow(cs)) {
        return FALSE;
    }
    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.lpszClass = AfxRegisterWndClass(
        CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
        AfxGetApp()->LoadStandardCursor(IDC_CROSS),
        (HBRUSH)(COLOR_BTNFACE + 1));
    return TRUE;
}

BEGIN_MESSAGE_MAP(HFDrawWindowContainer, CWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_MOUSEMOVE()
    ON_WM_MBUTTONDOWN()
    ON_WM_MBUTTONUP()
END_MESSAGE_MAP()

// HFDrawWindowContainer message handlers

int HFDrawWindowContainer::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_dispImage = new HFImageDisplay;
    m_dispImage->CreateHFImageDisplay(CPoint(0, 0), this);

    return 0;
}

void HFDrawWindowContainer::OnMButtonDown(UINT nFlags, CPoint point) {
    CWnd::OnMButtonDown(nFlags, point);
    m_infobar->SetHFDrawWindowInforBarText(HFSTRC(IDS_DRAWWINDOW_MBDOWN));
    m_ptPrevCursor = point;
    SetCapture();
    m_hPrevCursor = ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
}

void HFDrawWindowContainer::OnMButtonUp(UINT nFlags, CPoint point) {
    CWnd::OnMButtonUp(nFlags, point);

    ReleaseCapture();
    ::SetCursor(m_hPrevCursor);

}

void HFDrawWindowContainer::OnMouseMove(UINT nFlags, CPoint point) {
    CWnd::OnMouseMove(nFlags, point);

    if (nFlags & MK_MBUTTON) {
        CRect rcNewDiag = ui::GetWindowCRectInParent(m_dispImage);
        CRect rcNewHort = ui::GetWindowCRectInParent(m_dispImage);
        CRect rcNewVert = ui::GetWindowCRectInParent(m_dispImage);

        CSize csOffset = point - m_ptPrevCursor;
        rcNewDiag.OffsetRect(csOffset);
        rcNewHort.OffsetRect(csOffset.cx, 0);
        rcNewVert.OffsetRect(0, csOffset.cy);

        CRect rcNow;
        GetClientRect(rcNow);
        rcNow.MoveToXY(0, 0);
        if (ui::CRect1InsideCRect2(rcNow, rcNewDiag)) {
            m_dispImage->SetWindowPos(&wndTop, rcNewDiag.left, rcNewDiag.top, rcNewDiag.Width(), rcNewDiag.Height(), SWP_NOSIZE);
        }
        else if (ui::CRect1InsideCRect2(rcNow, rcNewHort)) {
            m_dispImage->SetWindowPos(&wndTop, rcNewHort.left, rcNewHort.top, rcNewHort.Width(), rcNewHort.Height(), SWP_NOSIZE);
        }
        else if (ui::CRect1InsideCRect2(rcNow, rcNewVert)) {
            m_dispImage->SetWindowPos(&wndTop, rcNewVert.left, rcNewVert.top, rcNewVert.Width(), rcNewVert.Height(), SWP_NOSIZE);
        }

        m_ptPrevCursor = point;
    }
    else {
        CPoint ptWRTImage = point;
        CRect rcImage = ui::GetWindowCRectInParent(m_dispImage);
        if (rcImage.PtInRect(point)) {
            ptWRTImage -= rcImage.TopLeft();
            ptWRTImage = m_dispImage->DisplayToLogical(ptWRTImage);
            CString sTemp;
            sTemp.Format(HFSTRC(IDS_DRAWWINDOW_INFOBAR_PANE1), ptWRTImage.x, ptWRTImage.y);
            m_infobar->SetHFDrawWindowInforBarText(sTemp, 0);
        }
        else {
            m_infobar->SetHFDrawWindowInforBarText(HFSTRC(IDS_DRAWWINDOW_INFOBAR_NOTINIMAGE));
        }
    }
}

void HFDrawWindowContainer::OnRButtonDblClk(UINT nFlags, CPoint point) {
    CWnd::OnRButtonDblClk(nFlags, point);
    RestoreZoom();
}


void HFDrawWindowContainer::OnSize(UINT nType, int cx, int cy) {
    CWnd::OnSize(nType, cx, cy);
    CRect rcImage = ui::GetWindowCRectInParent(m_dispImage);
    CRect rcDraw;
    GetClientRect(rcDraw);

    LONG lHOffset = 0;
    if (rcDraw.left < rcImage.left && rcDraw.right < rcImage.right
        || rcDraw.left > rcImage.left && rcDraw.right > rcImage.right) {
        lHOffset = rcDraw.right - rcImage.right;
    }
    LONG lVOffset = 0;
    if (rcDraw.top < rcImage.top && rcDraw.bottom < rcImage.bottom
        || rcDraw.top > rcImage.top && rcDraw.bottom > rcImage.bottom) {
        lVOffset = rcDraw.bottom - rcImage.bottom;
    }

    m_dispImage->SetWindowPos(&wndTop, rcImage.left + lHOffset, rcImage.top + lVOffset, rcImage.Width(), rcImage.Height(), SWP_NOSIZE);
}
