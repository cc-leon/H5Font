#include "pch.h"

#include "HFMainWindow.h"
#include "HFDrawWindow.h"
#include "../H5FontFileIO/HFBinFilesInfo.h"

HFMainWindow::HFMainWindow() 
    : m_logWnd(NULL), m_drawWnd(NULL), m_mnMain() {

}

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
        // TODO: m_mnMain.CheckMenuItem(IDM_WINDOWS_DRAW, MF_CHECKED);
        m_drawWnd->ShowWindow(SW_SHOW);
    }
    else {
        // TODO: m_mnMain.CheckMenuItem(IDM_WINDOWS_DRAW, MF_UNCHECKED);
        m_drawWnd->ShowWindow(SW_HIDE);
    }
}

BOOL HFMainWindow::CreateHFMainWindow() {
    return Create(
        NULL, ui::LoadRcString(IDS_MAINWINDOW_TITLE),
        WS_CAPTION | WS_MINIMIZEBOX | WS_OVERLAPPED | WS_SYSMENU | WS_POPUP,
        HFUIC::MainWindow::Size);
}

BOOL HFMainWindow::PreCreateWindow(CREATESTRUCT& cs) {
    if (!CFrameWnd::PreCreateWindow(cs)) {
        return FALSE;
    }

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.cx = HFUIC::MainWindow::Size.Width();
    cs.cy = HFUIC::MainWindow::Size.Height();
    cs.lpszClass = AfxRegisterWndClass(
        CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
        AfxGetApp()->LoadStandardCursor(IDC_ARROW),
        (HBRUSH)(COLOR_BTNFACE + 1));

    return TRUE;
}

BEGIN_MESSAGE_MAP(HFMainWindow, CFrameWnd)
    ON_WM_CLOSE()
    ON_WM_CREATE()

    // Buttom message
    ON_BN_CLICKED(HFUIC::MainWindow::ID_btnBrowsePak, &HFMainWindow::OnBtnBrowsePakClicked)
    ON_BN_CLICKED(HFUIC::MainWindow::ID_btnItalic, &HFMainWindow::OnBtnItalicClicked)
    ON_BN_CLICKED(HFUIC::MainWindow::ID_btnUnderline, &HFMainWindow::OnBtnUnderlineClicked)
    ON_BN_CLICKED(HFUIC::MainWindow::ID_btnRun, &HFMainWindow::OnBtnRunClicked)
    ON_BN_CLICKED(HFUIC::MainWindow::ID_btnPreviewDraw, &HFMainWindow::OnBtnPreviewDrawClicked)
    ON_BN_CLICKED(HFUIC::MainWindow::ID_btnPackage, &HFMainWindow::OnBtnPackageClicked)
    ON_BN_CLICKED(HFUIC::MainWindow::ID_btnOpenFolder, &HFMainWindow::OnBtnOpenFolderClicked)

    // Text and dropdownlist change functions
    ON_EN_CHANGE(HFUIC::MainWindow::ID_txtPak, &HFMainWindow::OnTxtPakChange)
    ON_EN_CHANGE(HFUIC::MainWindow::ID_txtBold, &HFMainWindow::OnTxtBoldChange)
    ON_EN_CHANGE(HFUIC::MainWindow::ID_txtSize, &HFMainWindow::OnTxtSizeChange)
    ON_CBN_SELCHANGE(HFUIC::MainWindow::ID_ddlFontSelect, &HFMainWindow::OnDdlFontSelectChange)
    ON_CBN_SELCHANGE(HFUIC::MainWindow::ID_ddlHeaderSelect, &HFMainWindow::OnDdlHeaderSelectChange)

    // Menu messages
    ON_COMMAND(IDM_WINDOWS_LOG, &HFMainWindow::OnWindowsLog)
    ON_COMMAND(IDM_HELP_README, &HFMainWindow::OnHelpReadme)
    ON_COMMAND(IDM_HELP_ONLINE, &HFMainWindow::OnHelpOnline)
    ON_COMMAND(IDM_HELP_ABOUT, &HFMainWindow::OnHelpAbout)
    ON_COMMAND(IDM_LANGUAGE_ENGLISH, &HFMainWindow::OnLanguageEnglish)
    ON_COMMAND(IDM_LANGUAGE_CHINESE, &HFMainWindow::OnLanguageChinese)

    // Custom message
    ON_MESSAGE(HFUIC::WindowMessage::MENU_WINDOWS_LOG, &HFMainWindow::OnWindowlog)

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
    SetFont(&HFUIC::Font.NORMAL_FONT);

    m_mnMain.LoadMenu(IDR_MAINWINDOW);
    SetMenu(&m_mnMain);


    m_logWnd = new HFLogWindow;
    m_logWnd->CreateHFLogWindow(this);
    m_logWnd->ShowWindow(SW_SHOW);
    m_logWnd->UpdateWindow();

    m_drawWnd = new HFDrawWindow;
    m_drawWnd->CreateHFDrawWindow(this);
    m_drawWnd->ShowWindow(SW_SHOW);
    m_drawWnd->UpdateWindow();


#define CREATE_GROUP_CTRL(ctrl, text, rect) \
    ctrl.CreateHFGroupBox(text, this, rect);\
    ctrl.SetFont(&HFUIC::Font.BOLD_FONT)

#define CREATE_PUSHBUTTON(ctrl, text, rect, parent, nID) \
    ctrl.Create(text, BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD, rect, &parent, nID); \
    ctrl.SetFont(&HFUIC::Font.NORMAL_FONT)

#define CREATE_LABEL(ctrl, text, rect, parent) \
    ctrl.Create(text, SS_SIMPLE | WS_VISIBLE | WS_CHILD, rect, &parent); \
    ctrl.SetFont(&HFUIC::Font.NORMAL_FONT)

#define CREATE_EDIT(ctrl, text, rect, parent, nID) \
    ctrl.Create(WS_VISIBLE | WS_BORDER | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL, rect, &parent, nID); \
    ctrl.SetWindowText(text); \
    ctrl.SetFont(&HFUIC::Font.NORMAL_FONT)

#define CREATE_NUMEDIT(ctrl, text, rect, parent, nID) \
    ctrl.Create(WS_VISIBLE | WS_BORDER | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_NUMBER, rect, &parent, nID); \
    ctrl.SetWindowText(text); \
    ctrl.SetFont(&HFUIC::Font.NORMAL_FONT)

#define CREATE_CHECKBUTTON(ctrl, text, rect, parent, nID) \
    ctrl.Create(text, WS_VISIBLE | WS_CHILD | BS_CHECKBOX, rect, &parent, nID); \
    ctrl.SetFont(&HFUIC::Font.NORMAL_FONT);

    CREATE_GROUP_CTRL(UIStep1.grp, ui::LoadRcString(IDS_MAINWINDOW_STEP1_TITLE), HFUIC::MainWindow::UIStep1::grpRect);
    {
        CREATE_LABEL(
            UIStep1.lblPak, ui::LoadRcString(IDS_MAINWINDOW_STEP1_LBLPAK),
            HFUIC::MainWindow::UIStep1::lblPakRect, UIStep1.grp);
        CREATE_EDIT(
            UIStep1.txtPak, _T(""), HFUIC::MainWindow::UIStep1::txtPakRect,
            UIStep1.grp, HFUIC::MainWindow::ID_txtPak);
        CREATE_PUSHBUTTON(
            UIStep1.btnBrowsePak, ui::LoadRcString(IDS_MAINWINDOW_STEP1_BROWSE),
            HFUIC::MainWindow::UIStep1::btnBrowsePakRect,
            UIStep1.grp, HFUIC::MainWindow::ID_btnBrowsePak);
    }

    CREATE_GROUP_CTRL(UIStep2.grp, ui::LoadRcString(IDS_MAINWINDOW_STEP2_TITLE), HFUIC::MainWindow::UIStep2::grpRect);
    {
        CREATE_LABEL(
            UIStep2.lblHeaderSelect, ui::LoadRcString(IDS_MAINWINDOW_STEP2_LBLHEADERSELECT),
            HFUIC::MainWindow::UIStep2::lblHeaderSelectRect, UIStep2.grp);
        UIStep2.ddlHeaderSelect.CreateHFFHeaderDropDownList(
            HFUIC::MainWindow::UIStep2::ddlHeaderSelectRect, &UIStep2.grp,
            HFUIC::MainWindow::ID_ddlHeaderSelect);
        UIStep2.lblHeaderSelect.SetFont(&HFUIC::Font.BOLD_FONT);
        UIStep2.ddlHeaderSelect.SetFont(&HFUIC::Font.BOLD_FONT);
        CREATE_LABEL(
            UIStep2.lblSplitter,
            _T("-------------------------------------------------------------------------------------------------------------"),
            HFUIC::MainWindow::UIStep2::lblSplitterRect, UIStep2.grp);

        CREATE_LABEL(
            UIStep2.lblFontSelect, ui::LoadRcString(IDS_MAINWINDOW_STEP2_LBLFONTSELECT),
            HFUIC::MainWindow::UIStep2::lblFontSelectRect, UIStep2.grp);
        UIStep2.ddlFontSelect.CreateHFFontDropDownList(
            HFUIC::MainWindow::UIStep2::ddlFontSelectRect,
            &UIStep2.grp, HFUIC::MainWindow::ID_ddlFontSelect);
        UIStep2.ddlFontSelect.SetFont(&HFUIC::Font.NORMAL_FONT);

        CREATE_LABEL(
            UIStep2.lblSize, ui::LoadRcString(IDS_MAINWINDOW_STEP2_LBLSIZE),
            HFUIC::MainWindow::UIStep2::lblSizeRect, UIStep2.grp);
        CREATE_NUMEDIT(
            UIStep2.txtSize, _T(""), HFUIC::MainWindow::UIStep2::txtSizeRect,
            UIStep2.grp, HFUIC::MainWindow::ID_txtSize);
        UIStep2.txtSize.LimitText(2);

        CREATE_LABEL(
            UIStep2.lblBold, ui::LoadRcString(IDS_MAINWINDOW_STEP2_LBLBOLD),
            HFUIC::MainWindow::UIStep2::lblBoldRect, UIStep2.grp);
        CREATE_NUMEDIT(
            UIStep2.txtBold, _T(""), HFUIC::MainWindow::UIStep2::txtBoldRect,
            UIStep2.grp, HFUIC::MainWindow::ID_txtBold);
        UIStep2.txtBold.LimitText(4);

        CREATE_CHECKBUTTON(
            UIStep2.btnItalic, ui::LoadRcString(IDS_MAINWINDOW_STEP2_BTNITALIC),
            HFUIC::MainWindow::UIStep2::btnItalicRect, UIStep2.grp, HFUIC::MainWindow::ID_btnItalic);
        CREATE_CHECKBUTTON(
            UIStep2.btnUnderline, ui::LoadRcString(IDS_MAINWINDOW_STEP2_BTNUNDERLINE),
            HFUIC::MainWindow::UIStep2::btnUnderlineRect, UIStep2.grp, HFUIC::MainWindow::ID_btnUnderline);
        UIStep2.lblPreview.Create(
            _T(""),
            SS_SUNKEN | WS_VISIBLE | WS_CHILD, 
            HFUIC::MainWindow::UIStep2::lblPreviewRect, &UIStep2.grp);
    }

    CREATE_GROUP_CTRL(UIStep3.grp, ui::LoadRcString(IDS_MAINWINDOW_STEP3_TITLE), HFUIC::MainWindow::UIStep3::grpRect);
    {
        CREATE_PUSHBUTTON(
            UIStep3.btnRun, ui::LoadRcString(IDS_MAINWINDOW_STEP3_BTNRUN),
            HFUIC::MainWindow::UIStep3::btnRunRect, UIStep3.grp, HFUIC::MainWindow::ID_btnRun);
        CREATE_PUSHBUTTON(
            UIStep3.btnPreviewDraw, ui::LoadRcString(IDS_MAINWINDOW_STEP3_PREVIEW),
            HFUIC::MainWindow::UIStep3::btnPreviewDrawRect, UIStep3.grp, HFUIC::MainWindow::ID_btnPreviewDraw);
    }

    CREATE_GROUP_CTRL(UIStep4.grp, ui::LoadRcString(IDS_MAINWINDOW_STEP4_TITLE), HFUIC::MainWindow::UIStep4::grpRect);
    {
        CREATE_PUSHBUTTON(
            UIStep4.btnPackage, ui::LoadRcString(IDS_MAINWINDOW_STEP4_BTNPACKAGE),
            HFUIC::MainWindow::UIStep4::btnPackageRect, UIStep4.grp, HFUIC::MainWindow::ID_btnPackage);
        CREATE_PUSHBUTTON(
            UIStep4.btnOpenFolder, ui::LoadRcString(IDS_MAINWINDOW_STEP4_BTNOPENFOLDER),
            HFUIC::MainWindow::UIStep4::btnOpenFolderRect, UIStep4.grp, HFUIC::MainWindow::ID_btnOpenFolder);
    }

#undef CREATE_GROUP_CTRL
#undef CREATE_PUSHBUTTON
#undef CREATE_LABEL
#undef CREATE_EDIT
#undef CREATE_NUMEDIT
#undef CREATE_CHECKBUTTON

    m_ttcMain.Create(this, TTS_ALWAYSTIP);
    m_ttcMain.AddToolThatWorks(_T(""), &UIStep1.txtPak);
    m_ttcMain.AddToolThatWorks(_T(""), &UIStep2.ddlFontSelect);
    m_ttcMain.AddToolThatWorks(_T(""), &UIStep2.ddlHeaderSelect);
    m_ttcMain.Activate(TRUE);
    return 0;
}

void HFMainWindow::OnBtnBrowsePakClicked() {
    CFileDialog cfdDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST,ui::LoadRcString(IDS_MAINWINDOW_STEP1_PAKFILTER), this);
    if (cfdDlg.DoModal() == IDOK) {
        UIStep1.txtPak.SetWindowText(cfdDlg.GetFileName());
    }
}

void HFMainWindow::OnBtnItalicClicked() {
    LOG.log(_T("Italic clicked"));
}

void HFMainWindow::OnBtnUnderlineClicked() {
    LOG.log(_T("Underline clicked"));
}

void HFMainWindow::OnBtnRunClicked() {
    LOG.log(_T("Run clicked"));
}

void HFMainWindow::OnBtnPreviewDrawClicked() {
    LOG.log(_T("Preview clicked"));
}

void HFMainWindow::OnBtnPackageClicked() {
    LOG.log(_T("Package clicked"));
}

void HFMainWindow::OnBtnOpenFolderClicked() {
    LOG.log(_T("Open folder clicked"));
}

void HFMainWindow::OnTxtPakChange() {
    LOG.log(_T("txtPak changed"));
}

void HFMainWindow::OnTxtBoldChange() {
    LOG.log(_T("txtBold changed"));
}

void HFMainWindow::OnTxtSizeChange() {
    LOG.log(_T("txtSize changed"));
}


void HFMainWindow::OnDdlHeaderSelectChange() {
    LOG.log(_T("ddlHeaderSelect changed"));
}

void HFMainWindow::OnDdlFontSelectChange() {
    LOG.log(_T("ddlFontSelect changed"));
}

void HFMainWindow::OnWindowsLog() {
    SetMenuWindowsLogChecked(!GetMenuChecked(IDM_WINDOWS_LOG));
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

LRESULT HFMainWindow::OnWindowlog(WPARAM wParam, LPARAM lParam) {
    SetMenuWindowsLogChecked(FALSE);
    return 0;
}

LRESULT HFMainWindow::OnWindowdraw(WPARAM wParam, LPARAM lParam) {
    SetMenuWindowsDrawChecked(FALSE);
    return 0;
}
