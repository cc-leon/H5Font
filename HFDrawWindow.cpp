#include "pch.h"
#include "HFDrawWindow.h"
#include "HFLogger.h"

// HFDrawWindow
HFDrawWindow::HFDrawWindow() 
    : CFrameWnd(), m_dispImage(NULL), m_hPrevCursor(NULL){ }
HFDrawWindow::~HFDrawWindow() {}

VOID MoveImage(CPoint CONST& ptPrev, CPoint CONST& ptCurr) {
}

VOID RectOkay(CRect CONST& rcNew) {
}


BOOL HFDrawWindow::PreCreateWindow(CREATESTRUCT& cs) {
    if (!CFrameWnd::PreCreateWindow(cs)) {
        return FALSE;
    }

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.cx = UIConst::DrawWindow::Size.Width();
    cs.cy = UIConst::DrawWindow::Size.Height();
    cs.lpszClass = AfxRegisterWndClass(
        CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
        AfxGetApp()->LoadStandardCursor(IDC_CROSS),
        (HBRUSH)(COLOR_BTNFACE + 1));

    return TRUE;
}

BEGIN_MESSAGE_MAP(HFDrawWindow, CFrameWnd)
    ON_WM_CLOSE()
    ON_WM_CREATE()
    ON_WM_MOUSEWHEEL()
    ON_WM_MBUTTONDOWN()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_MOUSEMOVE()
    ON_WM_MBUTTONUP()
END_MESSAGE_MAP()

void HFDrawWindow::OnClose() {
    ShowWindow(SW_HIDE);
}

int HFDrawWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_dispImage = new HFBmpDisplay;
    CRect rc;
    GetWindowRect(rc);
    CRect newRc;
    newRc.top = 0;
    newRc.left = 0;
    newRc.right = rc.Width();
    newRc.bottom = rc.Height();
    CImage * aa = new CImage;
    if (aa->Load(_T("kk.png")) != S_OK) {
        ::AfxMessageBox(_T("Not loaded"));
    }
    HFBitmap* bb = new HFBitmap;
    bb->Attach(aa->Detach());
    BITMAP cc;
    bb->GetBitmap(&cc);
    m_dispImage->Create(CPoint(0, 0), bb, this);
    return 0;
}

void HFDrawWindow::OnMButtonDown(UINT nFlags, CPoint point) {
    CFrameWnd::OnMButtonDown(nFlags, point);
    m_ptPrevCursor = point;
    SetCapture();
    m_hPrevCursor = ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
}

void HFDrawWindow::OnMButtonUp(UINT nFlags, CPoint point) {
    CFrameWnd::OnMButtonUp(nFlags, point);
    ReleaseCapture();
    ::SetCursor(m_hPrevCursor);
}

void HFDrawWindow::OnMouseMove(UINT nFlags, CPoint point) {
    CFrameWnd::OnMouseMove(nFlags, point);
    if (nFlags & MK_MBUTTON) {
        CString a;
        a.Format(_T("Mouse move, %d, %d"), point.x, point.y);
        LOG.log(a);
        CRect rcNew = ui::GetWindowCRectInParent(m_dispImage);
        rcNew.OffsetRect(point - m_ptPrevCursor);
        CRect rcNow;
        GetClientRect(rcNow);
        rcNow.MoveToXY(0, 0);
        if (ui::CRect1InsideCRect2(rcNow, rcNew)) {
            m_dispImage->SetWindowPos(&wndTop, rcNew.left, rcNew.top, rcNew.Width(), rcNew.Height(), NULL);
        }
        m_ptPrevCursor = point;
    }
}

BOOL HFDrawWindow::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
    ScreenToClient(&pt);

    CRect rcImage = ui::GetWindowCRectInParent(m_dispImage);
    if (rcImage.PtInRect(pt) && zDelta != 0) {
        CRect rcImage = ui::GetWindowCRectInParent(m_dispImage);
        m_dispImage->SetZoom(
            m_dispImage->GetZoom() + zDelta / WHEEL_DELTA * 10,
            pt - rcImage.TopLeft());
    }
    return CFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void HFDrawWindow::OnRButtonDblClk(UINT nFlags, CPoint point) {
    CFrameWnd::OnRButtonDblClk(nFlags, point);
    m_dispImage->SetZoom(100, CPoint(0,0));
    m_dispImage->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
}
