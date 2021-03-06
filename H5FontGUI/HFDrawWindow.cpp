#include "pch.h"
#include "HFDrawWindow.h"

// HFDrawWindow
HFDrawWindow::HFDrawWindow()
    : CFrameWnd(), m_dcDrawCentre(NULL), m_iDrawCentreIndex(0) {}

HFDrawWindow::~HFDrawWindow() {}

BOOL HFDrawWindow::CreateHFDrawWindow(CWnd* pParentWnd) {
    return Create(
        NULL, HFSTRC(IDS_HFDRAWWINDOW_TITLE),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX,
        HFUIC::DrawWindow::Size, pParentWnd);
}

void HFDrawWindow::GetActiveArea(LPRECT lpRect) CONST {
    CRect rcResult;
    GetClientRect(rcResult);
    CRect rcInfoBar;
    m_infoBar.GetWindowRect(rcInfoBar);
    rcResult.bottom -= rcInfoBar.Height();
    rcResult.left += 1;
    rcResult.top += 1;
    rcResult.right -= 2;
    rcResult.bottom -= 2;
    *lpRect = rcResult;
}

VOID HFDrawWindow::SetDrawCentre(HFDrawDCsCentre* dcDrawCentre) {
    m_dcDrawCentre = dcDrawCentre;
    m_iDrawCentreIndex = 0;
    m_container.SetMemDC(&(*m_dcDrawCentre)[m_iDrawCentreIndex]);
    SetTitle();
}

VOID HFDrawWindow::SetTitle() {
    CString sTemp;
    sTemp.Format(
        HFSTRC(IDS_DRAWWINDOW_TITLE),
        (*m_dcDrawCentre)[m_iDrawCentreIndex].GetFontInfo()->szFacenam,
        (*m_dcDrawCentre)[m_iDrawCentreIndex].GetFontInfo()->nHeight,
        (*m_dcDrawCentre)[m_iDrawCentreIndex].GetFontInfo()->nWeight,
        HFLC::header::CODE_TO_LPTSTR[m_iDrawCentreIndex],
        (*m_dcDrawCentre)[m_iDrawCentreIndex].GetUnicodeCount());
    SetWindowText(sTemp);
}

BOOL HFDrawWindow::PreCreateWindow(CREATESTRUCT& cs) {
    if (!CFrameWnd::PreCreateWindow(cs)) {
        return FALSE;
    }

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.cx = HFUIC::DrawWindow::Size.Width();
    cs.cy = HFUIC::DrawWindow::Size.Height();
    cs.lpszClass = AfxRegisterWndClass(
        CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
        AfxGetApp()->LoadStandardCursor(IDC_CROSS),
        (HBRUSH)(COLOR_BTNFACE + 1));

    return TRUE;
}

BEGIN_MESSAGE_MAP(HFDrawWindow, CFrameWnd)
    ON_WM_CLOSE()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_MOUSEWHEEL()
    ON_COMMAND(IDM_DRAWWINDOW_100, &HFDrawWindow::OnDrawwindow100)
    ON_COMMAND(IDM_DRAWWINDOW_NEXT, &HFDrawWindow::OnDrawwindowNext)
    ON_COMMAND(IDM_DRAWWINDOW_PREV, &HFDrawWindow::OnDrawwindowPrev)
    ON_COMMAND(IDM_DRAWWINDOW_ZOOMIN, &HFDrawWindow::OnDrawwindowZoomin)
    ON_COMMAND(IDM_DRAWWINDOW_ZOOMOUT, &HFDrawWindow::OnDrawwindowZoomout)
    ON_WM_MENUSELECT()
END_MESSAGE_MAP()

void HFDrawWindow::OnClose() {
    AfxGetMainWnd()->SendMessage(HFUIC::WindowMessage::MENU_WINDOWS_DRAW);
}

int HFDrawWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MAIN_ICON));
    SetIcon(hIcon, FALSE);

    m_infoBar.CreateHFDrawWindowInfoBar(this);

    m_container.CreateHFDrawWindowContainer(this, &m_infoBar);
    m_container.ShowWindow(SW_SHOW);


    m_mnDrawWindow.LoadMenu(IDR_DRAWWINDOW);
    SetMenu(&m_mnDrawWindow);

    return 0;
}

void HFDrawWindow::OnSize(UINT nType, int cx, int cy) {
    CFrameWnd::OnSize(nType, cx, cy);

    m_infoBar.UpdateFromParent();
    CRect rect = ui::GetWindowCRectInParent(&m_infoBar);

    CRect rcDraw;
    GetActiveArea(rcDraw);
    m_container.SetWindowPos(&wndBottom, rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom, NULL);
}


BOOL HFDrawWindow::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
    // TODO: Add your message handler code here and/or call default
    m_container.OnMouseWheel(nFlags, zDelta, pt);
    return CFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void HFDrawWindow::OnDrawwindow100() {
    m_container.RestoreZoom();
}

void HFDrawWindow::OnDrawwindowNext() {
    if (m_iDrawCentreIndex < HFLC::header::HEADER_COUNT - 1) {
        m_container.SetMemDC(&(*m_dcDrawCentre)[++m_iDrawCentreIndex]);
        SetTitle();
    }
}


void HFDrawWindow::OnDrawwindowPrev() {
    if (m_iDrawCentreIndex > 0) {
        m_container.SetMemDC(&(*m_dcDrawCentre)[--m_iDrawCentreIndex]);
        SetTitle();
    }
}

void HFDrawWindow::OnDrawwindowZoomin() {
    m_container.OnMouseWheel(NULL, WHEEL_DELTA, m_container.GetImageCenter());
}

void HFDrawWindow::OnDrawwindowZoomout() {
    m_container.OnMouseWheel(NULL, -WHEEL_DELTA, m_container.GetImageCenter());
}

void HFDrawWindow::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu){
    CFrameWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
}
