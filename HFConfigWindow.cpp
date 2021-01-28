#include "pch.h"
#include "HFConfigWindow.h"
#include "HFDrawWindow.h"

HFConfigWindow::HFConfigWindow() : m_logWnd(NULL), m_drawWnd(NULL) {}

HFConfigWindow::~HFConfigWindow() {}

CWnd * HFConfigWindow::GetLogWnd() {
    return m_logWnd;
}


BOOL HFConfigWindow::PreCreateWindow(CREATESTRUCT& cs) {
    if (!CFrameWnd::PreCreateWindow(cs)) {
        return FALSE;
    }

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.cx = UIConst::ConfigWindow::Size.Width();
    cs.cy = UIConst::ConfigWindow::Size.Height();
    cs.lpszClass = AfxRegisterWndClass(
        CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
        AfxGetApp()->LoadStandardCursor(IDC_ARROW),
        (HBRUSH)(COLOR_BTNFACE + 1));

    return TRUE;
}

BEGIN_MESSAGE_MAP(HFConfigWindow, CFrameWnd)
    ON_WM_CLOSE()
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// HFConfigWindow message handlers

void HFConfigWindow::OnClose() {
    m_logWnd->DestroyWindow();
    m_drawWnd->DestroyWindow();
    CFrameWnd::OnClose();
}

int HFConfigWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    m_logWnd = new HFLogWindow;
    if (!m_logWnd) {
        return FALSE;
    }
    m_logWnd->Create(NULL, _T("H5FontLogWindow"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX,
        UIConst::LogWindow::Size, this);
    m_logWnd->ShowWindow(SW_SHOW);
    m_logWnd->UpdateWindow();

    m_drawWnd = new HFDrawWindow;
    if (m_drawWnd == NULL) {
        return FALSE;
    }
    m_drawWnd->Create(NULL, _T("H5FontDrawWindow"), 
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX,
        UIConst::DrawWindow::Size, this);
    m_drawWnd->ShowWindow(SW_SHOW);
    m_drawWnd->UpdateWindow();

    return 0;
}

BOOL HFConfigWindow::OnEraseBkgnd(CDC* pDC){
    return CFrameWnd::OnEraseBkgnd(pDC);
}
