// HFLogWindow.cpp : implementation file
//

#include "pch.h"
#include "HFLogWindow.h"
#include "../H5FontLogger/HFLogLib.h"

// HFLogWindow
HFLogWindow::HFLogWindow() {}

HFLogWindow::~HFLogWindow() {}

BOOL HFLogWindow::CreateHFLogWindow(CWnd * pParentWnd) {
    return Create(NULL, _T("HFLogWindow"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX,
        HFUIC::LogWindow::Size, pParentWnd);
}

BOOL HFLogWindow::PreCreateWindow(CREATESTRUCT& cs) {
    if (!CFrameWnd::PreCreateWindow(cs)) {
        return FALSE;
    }

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.cx = HFUIC::LogWindow::Size.Width();
    cs.cy = HFUIC::LogWindow::Size.Height();
    cs.lpszClass = AfxRegisterWndClass(
        CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
        AfxGetApp()->LoadStandardCursor(IDC_ARROW),
        (HBRUSH)(COLOR_BTNFACE + 1));

    return TRUE;
}


BEGIN_MESSAGE_MAP(HFLogWindow, CFrameWnd)
    ON_WM_CLOSE()
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// HFLogWindow message handlers

void HFLogWindow::OnClose() {
    AfxGetMainWnd()->SendMessage(HFUIC::WindowMessage::MENU_WINDOWS_LOG);
}

int HFLogWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MAIN_ICON));
    SetIcon(hIcon, FALSE);

    m_txtLog.CreateHFRichTextBox(this, CRect(0, 0, lpCreateStruct->cx, lpCreateStruct->cy));

    LOG.Initialize(&m_txtLog);
    LOG.log(HFSTRC(IDS_LOG_APPSTART), HFUIC::Color::GREEN);

    return 0;
}

void HFLogWindow::OnSize(UINT nType, int cx, int cy) {
    CFrameWnd::OnSize(nType, cx, cy);
    m_txtLog.SetWindowPos(&wndTop, 0, 0, cx, cy, NULL);
}
