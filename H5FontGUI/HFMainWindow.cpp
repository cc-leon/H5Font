#include "pch.h"

#include "HFMainWindow.h"
#include "HFDrawWindow.h"
#include "../H5FontFileIO/HFBinFileInfo.h"

HFMainWindow::HFMainWindow() 
    : m_logWnd(NULL), m_drawWnd(NULL), m_mnMain(){}

HFMainWindow::~HFMainWindow() {}

CWnd * HFMainWindow::GetLogWnd() {
    return m_logWnd;
}

BOOL HFMainWindow::GetMenuChecked(UINT uiMenuID) {
    return (m_mnMain.GetMenuState(uiMenuID, MF_BYCOMMAND) & MF_CHECKED) > 0;
}

VOID HFMainWindow::SetMenuWindowsLogChecked(BOOL bChecked) {
    if (bChecked) {
        m_mnMain.CheckMenuItem(IDM_WINDOWS_LOG, MF_CHECKED);
        m_logWnd->ShowWindow(SW_SHOW);
    }
    else {
        m_mnMain.CheckMenuItem(IDM_WINDOWS_LOG, MF_UNCHECKED);
        m_logWnd->ShowWindow(SW_HIDE);
    }
}

VOID HFMainWindow::SetMenuWindowsDrawChecked(BOOL bChecked) {
    if (bChecked) {
        m_mnMain.CheckMenuItem(IDM_WINDOWS_DRAW, MF_CHECKED);
        m_drawWnd->ShowWindow(SW_SHOW);
    }
    else {
        m_mnMain.CheckMenuItem(IDM_WINDOWS_DRAW, MF_UNCHECKED);
        m_drawWnd->ShowWindow(SW_HIDE);
    }
}

BOOL HFMainWindow::CreateHFMainWindow() {
    return Create(
        NULL, CString(_T("H5FontConfigWindow")),
        WS_CAPTION | WS_MINIMIZEBOX | WS_OVERLAPPED | WS_SYSMENU,
        UIC::ConfigWindow::Size);
}

BOOL HFMainWindow::PreCreateWindow(CREATESTRUCT& cs) {
    if (!CFrameWnd::PreCreateWindow(cs)) {
        return FALSE;
    }

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.cx = UIC::ConfigWindow::Size.Width();
    cs.cy = UIC::ConfigWindow::Size.Height();
    cs.lpszClass = AfxRegisterWndClass(
        CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
        AfxGetApp()->LoadStandardCursor(IDC_ARROW),
        (HBRUSH)(COLOR_BTNFACE + 1));

    return TRUE;
}

BEGIN_MESSAGE_MAP(HFMainWindow, CFrameWnd)
    ON_WM_CLOSE()
    ON_WM_CREATE()

    ON_MESSAGE(UIC::WindowMessage::MENU_WINDOWS_LOG, &HFMainWindow::OnWindowlog)
    ON_MESSAGE(UIC::WindowMessage::MENU_WINDOWS_DRAW, &HFMainWindow::OnWindowdraw)

    // Menu messages
    ON_COMMAND(IDM_START_SETUP, &HFMainWindow::OnStartSetup)
    ON_COMMAND(IDM_START_RUN, &HFMainWindow::OnStartRun)
    ON_COMMAND(IDM_START_EXIT, &HFMainWindow::OnStartExit)
    ON_COMMAND(IDM_WINDOWS_LOG, &HFMainWindow::OnWindowsLog)
    ON_COMMAND(IDM_WINDOWS_DRAW, &HFMainWindow::OnWindowsDraw)
    ON_COMMAND(IDM_HELP_README, &HFMainWindow::OnHelpReadme)
    ON_COMMAND(IDM_HELP_ONLINE, &HFMainWindow::OnHelpOnline)
    ON_COMMAND(IDM_HELP_ABOUT, &HFMainWindow::OnHelpAbout)
    ON_COMMAND(IDM_LANGUAGE_ENGLISH, &HFMainWindow::OnLanguageEnglish)
    ON_COMMAND(IDM_LANGUAGE_CHINESE, &HFMainWindow::OnLanguageChinese)
    ON_BN_CLICKED(0x1111, ONMainBtnClicked)
    ON_WM_MENUSELECT()
END_MESSAGE_MAP()

// HFMainWindow message handlers

void HFMainWindow::OnClose() {
    m_logWnd->DestroyWindow();
    m_drawWnd->DestroyWindow();
    CFrameWnd::OnClose();
}

int HFMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    m_logWnd = new HFLogWindow;
    m_logWnd->CreateHFLogWindow(this);
    m_logWnd->ShowWindow(SW_SHOW);
    m_logWnd->UpdateWindow();

    m_drawWnd = new HFDrawWindow;
    m_drawWnd->CreateHFDrawWindow(this);
    m_drawWnd->ShowWindow(SW_SHOW);
    m_drawWnd->UpdateWindow();

    BOOL result = m_mnMain.LoadMenu(IDR_MAINWINDOW);
    result = SetMenu(&m_mnMain);

    m_btnMain.Create(_T("TEST"), WS_CHILD | WS_VISIBLE, CRect(10, 10, 100, 50), this, 0x1111);
    m_btnMain.ShowWindow(SW_SHOW);
    return 0;
}

afx_msg LRESULT HFMainWindow::OnWindowlog(WPARAM wParam, LPARAM lParam) {
    SetMenuWindowsLogChecked(FALSE);
    return 0;
}

afx_msg LRESULT HFMainWindow::OnWindowdraw(WPARAM wParam, LPARAM lParam) {
    SetMenuWindowsDrawChecked(FALSE);
    return 0;
}

void HFMainWindow::OnStartSetup() {
    // TODO: Add your command handler code here
}

void HFMainWindow::OnStartRun() {
    // TODO: Add your command handler code here
}

void HFMainWindow::OnStartExit() {
    // TODO: Add your command handler code here
}

void HFMainWindow::OnWindowsLog() {
    SetMenuWindowsLogChecked(!GetMenuChecked(IDM_WINDOWS_LOG));
}

void HFMainWindow::OnWindowsDraw() {
    SetMenuWindowsDrawChecked(!GetMenuChecked(IDM_WINDOWS_DRAW));
}

void HFMainWindow::OnHelpReadme() {
    // TODO: Add your command handler code here
}

void HFMainWindow::OnHelpOnline() {
    // TODO: Add your command handler code here
}

void HFMainWindow::OnHelpAbout() {
    // TODO: Add your command handler code here
}

void HFMainWindow::OnLanguageEnglish() {
    // TODO: Add your command handler code here
}

void HFMainWindow::OnLanguageChinese() {
    // TODO: Add your command handler code here
}


void HFMainWindow::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu) {
    CFrameWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
    CString a;
    a.Format(_T("Item %d is selected via menu"), nItemID);
    // TODO: Add your message handler code here
}

afx_msg void HFMainWindow::ONMainBtnClicked() {
    CString a;
    LOG.log(_T("Main button clicked"));
    //HFBinFileInfo t(_T("D:\\games\\TOE31"));
    //a.Format(_T("%d files and folders removed"), file::ClearFolder(_T("D:\\New folder\\")));
    //file::CreateFolderIfNotExists("D:\\a\\b");
    TCHAR result[4];
    file::CreateFolderIfNotExists(_T("D:\\a\\b\\c\\d\\e\\f\\g\\h\\i\\j\\k"));
}