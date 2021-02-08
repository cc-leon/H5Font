#include "pch.h"

#include "HFMainWindow.h"
#include "HFDrawWindow.h"
#include "../H5FontFileIO/HFBinFilesInfo.h"

HFMainWindow::HFMainWindow() 
    : m_logWnd(NULL), m_drawWnd(NULL), m_mnMain(), m_iFontIndex(0) {

    for (int i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        ::StringCchCopy(m_fiFonts[i].szFacenam, LF_FACESIZE, _T("ºÚÌå"));
        m_fiFonts[i].nPadding = 0;
        m_fiFonts[i].nVPosition = 5;
        m_fiFonts[i].nHeight = HFLC::header::DEFAULT_HEIGHT[i];
        m_fiFonts[i].nWeight = HFLC::header::DEFAULT_WEIGHT[i];
        m_fiFonts[i].bItalic = 0;
        m_fiFonts[i].bUnderline = 0;
    }
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

VOID HFMainWindow::SetBtnPreviewDrawChecked(BOOL bChecked) {
    if (bChecked) {
        m_drawWnd->ShowWindow(SW_SHOW);
    }
    else {
        m_drawWnd->ShowWindow(SW_HIDE);
    }
    UIStep3.btnPreviewDraw.SetCheck(bChecked);
}

VOID HFMainWindow::SetFontInfoToGUI(FONTINFO CONST& fi) {
    CString sTemp;
    UIStep2.ddlFontSelect.SetCurSel(UIStep2.ddlFontSelect.FindStringExact(0, fi.szFacenam));
    sTemp.Format(_T("%d"), fi.nPadding);
    UIStep2.txtPadding.SetWindowText(sTemp);
    sTemp.Format(_T("%d"), fi.nVPosition);
    UIStep2.txtVPosition.SetWindowText(sTemp);
    sTemp.Format(_T("%d"), fi.nHeight);
    UIStep2.txtSize.SetWindowText(sTemp);
    sTemp.Format(_T("%d"), fi.nHeight);
    UIStep2.txtSize.SetWindowText(sTemp);
    sTemp.Format(_T("%d"), fi.nWeight);
    UIStep2.txtBold.SetWindowText(sTemp);
    UIStep2.btnItalic.SetCheck(fi.bItalic);
    UIStep2.btnUnderline.SetCheck(fi.bUnderline);
}

VOID HFMainWindow::GetFontInfoFromGUI(FONTINFO& fi)  {
    CString sTemp;
    if (UIStep2.ddlFontSelect.GetCurSel() != LB_ERR) {
        UIStep2.ddlFontSelect.GetLBText(UIStep2.ddlFontSelect.GetCurSel(), sTemp);
    }
    ::StringCchCopy(fi.szFacenam, LF_FACESIZE, sTemp);
    UIStep2.txtPadding.GetWindowText(sTemp);
    fi.nPadding = ::StrToInt(sTemp);
    UIStep2.txtVPosition.GetWindowText(sTemp);
    fi.nVPosition = ::StrToInt(sTemp);
    UIStep2.txtSize.GetWindowText(sTemp);
    fi.nHeight = ::StrToInt(sTemp);
    UIStep2.txtBold.GetWindowText(sTemp);
    fi.nWeight = ::StrToInt(sTemp);
    fi.bItalic = UIStep2.btnItalic.GetCheck();
    fi.bUnderline = UIStep2.btnUnderline.GetCheck();
}

VOID HFMainWindow::UpdateLblPreview() {
    CString sSample(HFSTRC(IDS_MAINWINDOW_STEP2_SAMPLETEXT));
    LPCFONTINFO pfi = &m_fiFonts[m_iFontIndex];
    CFont newFont;
    newFont.CreateFont(
        pfi->nHeight,              // Height
        0,                         // Width
        0,                         // Escapement
        0,                         // OrientationME
        pfi->nWeight,              // Weight
        pfi->bItalic,              // Italic
        pfi->bUnderline,           // Underline
        0,                         // StrikeOut
        GB2312_CHARSET,              // CharSet
        OUT_DEVICE_PRECIS,         // OutPrecision
        CLIP_DEFAULT_PRECIS,       // ClipPrecision
        CLEARTYPE_QUALITY,         // Quality
        FIXED_PITCH || FF_MODERN,  // PitchAndFamily
        pfi->szFacenam);           // Facename
    UIStep2.lblPreview.SetWindowText(_T(""));
    UIStep2.lblPreview.SetFont(&newFont);
    UIStep2.lblPreview.SetWindowText(sSample);
    UIStep2.lblPreview.RedrawWindow();
}

CString HFMainWindow::ValidateTxtPak() {
    CString sTemp;
    CString sResult;
    UIStep1.txtPak.GetWindowText(sTemp);
    if (!file::FileExists(sTemp)) {
        sResult.Format(HFSTRC(IDS_MAINWINDOW_STEP1_INVALIDPAK), sTemp);
    }
    return sResult;
}

BOOL HFMainWindow::CreateHFMainWindow() {
    return Create(
        NULL, HFSTRC(IDS_MAINWINDOW_TITLE),
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
    ON_EN_KILLFOCUS(HFUIC::MainWindow::ID_txtPak, &HFMainWindow::OnTxtPakChange)
    ON_EN_KILLFOCUS(HFUIC::MainWindow::ID_txtPadding, &HFMainWindow::OnTxtPaddingChange)
    ON_EN_KILLFOCUS(HFUIC::MainWindow::ID_txtVPosition, &HFMainWindow::OnTxtVPositionChange)
    ON_EN_KILLFOCUS(HFUIC::MainWindow::ID_txtBold, &HFMainWindow::OnTxtBoldChange)
    ON_EN_KILLFOCUS(HFUIC::MainWindow::ID_txtSize, &HFMainWindow::OnTxtSizeChange)
    ON_CBN_SELENDOK(HFUIC::MainWindow::ID_ddlFontSelect, &HFMainWindow::OnDdlFontSelectChange)
    ON_CBN_SELENDOK(HFUIC::MainWindow::ID_ddlHeaderSelect, &HFMainWindow::OnDdlHeaderSelectChange)

    // Menu messages
    ON_COMMAND(IDM_WINDOWS_LOG, &HFMainWindow::OnWindowsLog)
    ON_COMMAND(IDM_HELP_README, &HFMainWindow::OnHelpReadme)
    ON_COMMAND(IDM_HELP_ONLINE, &HFMainWindow::OnHelpOnline)
    ON_COMMAND(IDM_HELP_ABOUT, &HFMainWindow::OnHelpAbout)
    ON_COMMAND(IDM_LANGUAGE_ENGLISH, &HFMainWindow::OnLanguageEnglish)
    ON_COMMAND(IDM_LANGUAGE_CHINESE, &HFMainWindow::OnLanguageChinese)

    // Custom message
    ON_MESSAGE(HFUIC::WindowMessage::MENU_WINDOWS_LOG, &HFMainWindow::OnWindowlog)
    ON_MESSAGE(HFUIC::WindowMessage::MENU_WINDOWS_DRAW, &HFMainWindow::OnWindowdraw)
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

    CREATE_GROUP_CTRL(UIStep1.grp, HFSTRC(IDS_MAINWINDOW_STEP1_TITLE), HFUIC::MainWindow::UIStep1::grpRect);
    {
        CREATE_LABEL(
            UIStep1.lblPak, HFSTRC(IDS_MAINWINDOW_STEP1_LBLPAK),
            HFUIC::MainWindow::UIStep1::lblPakRect, UIStep1.grp);
        CREATE_EDIT(
            UIStep1.txtPak, _T(""), HFUIC::MainWindow::UIStep1::txtPakRect,
            UIStep1.grp, HFUIC::MainWindow::ID_txtPak);
        CREATE_PUSHBUTTON(
            UIStep1.btnBrowsePak, HFSTRC(IDS_MAINWINDOW_STEP1_BROWSE),
            HFUIC::MainWindow::UIStep1::btnBrowsePakRect,
            UIStep1.grp, HFUIC::MainWindow::ID_btnBrowsePak);
    }

    CREATE_GROUP_CTRL(UIStep2.grp, HFSTRC(IDS_MAINWINDOW_STEP2_TITLE), HFUIC::MainWindow::UIStep2::grpRect);
    {
        CREATE_LABEL(
            UIStep2.lblHeaderSelect, HFSTRC(IDS_MAINWINDOW_STEP2_LBLHEADERSELECT),
            HFUIC::MainWindow::UIStep2::lblHeaderSelectRect, UIStep2.grp);
        UIStep2.ddlHeaderSelect.CreateHFFHeaderDropDownList(
            HFUIC::MainWindow::UIStep2::ddlHeaderSelectRect, &UIStep2.grp,
            HFUIC::MainWindow::ID_ddlHeaderSelect);
        UIStep2.lblHeaderSelect.SetFont(&HFUIC::Font.BOLD_FONT);
        UIStep2.ddlHeaderSelect.SetCurSel(0);
        UIStep2.ddlHeaderSelect.SetFont(&HFUIC::Font.BOLD_FONT);
        CREATE_LABEL(
            UIStep2.lblSplitter,
            _T("-------------------------------------------------------------------------------------------------------------"),
            HFUIC::MainWindow::UIStep2::lblSplitterRect, UIStep2.grp);

        CREATE_LABEL(
            UIStep2.lblFontSelect, HFSTRC(IDS_MAINWINDOW_STEP2_LBLFONTSELECT),
            HFUIC::MainWindow::UIStep2::lblFontSelectRect, UIStep2.grp);
        UIStep2.ddlFontSelect.CreateHFFontDropDownList(
            HFUIC::MainWindow::UIStep2::ddlFontSelectRect,
            &UIStep2.grp, HFUIC::MainWindow::ID_ddlFontSelect);
        UIStep2.ddlFontSelect.SetFont(&HFUIC::Font.NORMAL_FONT);

        CREATE_LABEL(
            UIStep2.lblPadding, HFSTRC(IDS_MAINWINDOW_STEP2_LBLPADDING),
            HFUIC::MainWindow::UIStep2::lblPaddingRect, UIStep2.grp);
        CREATE_NUMEDIT(
            UIStep2.txtPadding, _T(""), HFUIC::MainWindow::UIStep2::txtPaddingRect,
            UIStep2.grp, HFUIC::MainWindow::ID_txtPadding);
        UIStep2.txtPadding.LimitText(2);

        CREATE_LABEL(
            UIStep2.lblVPosition, HFSTRC(IDS_MAINWINDOW_STEP2_LBLVPOSITION),
            HFUIC::MainWindow::UIStep2::lblVPositionRect, UIStep2.grp);
        CREATE_NUMEDIT(
            UIStep2.txtVPosition, _T(""), HFUIC::MainWindow::UIStep2::txtVPositionRect,
            UIStep2.grp, HFUIC::MainWindow::ID_txtVPosition);
        UIStep2.txtVPosition.LimitText(2);

        CREATE_LABEL(
            UIStep2.lblSize, HFSTRC(IDS_MAINWINDOW_STEP2_LBLSIZE),
            HFUIC::MainWindow::UIStep2::lblSizeRect, UIStep2.grp);
        CREATE_NUMEDIT(
            UIStep2.txtSize, _T(""), HFUIC::MainWindow::UIStep2::txtSizeRect,
            UIStep2.grp, HFUIC::MainWindow::ID_txtSize);
        UIStep2.txtSize.LimitText(2);

        CREATE_LABEL(
            UIStep2.lblBold, HFSTRC(IDS_MAINWINDOW_STEP2_LBLBOLD),
            HFUIC::MainWindow::UIStep2::lblBoldRect, UIStep2.grp);
        CREATE_NUMEDIT(
            UIStep2.txtBold, _T(""), HFUIC::MainWindow::UIStep2::txtBoldRect,
            UIStep2.grp, HFUIC::MainWindow::ID_txtBold);
        UIStep2.txtBold.LimitText(3);

        CREATE_CHECKBUTTON(
            UIStep2.btnItalic, HFSTRC(IDS_MAINWINDOW_STEP2_BTNITALIC),
            HFUIC::MainWindow::UIStep2::btnItalicRect, UIStep2.grp, HFUIC::MainWindow::ID_btnItalic);
        CREATE_CHECKBUTTON(
            UIStep2.btnUnderline, HFSTRC(IDS_MAINWINDOW_STEP2_BTNUNDERLINE),
            HFUIC::MainWindow::UIStep2::btnUnderlineRect, UIStep2.grp, HFUIC::MainWindow::ID_btnUnderline);
        UIStep2.lblPreview.Create(
            _T(""),
            SS_SUNKEN | WS_VISIBLE | WS_CHILD, 
            HFUIC::MainWindow::UIStep2::lblPreviewRect, &UIStep2.grp);
    }

    CREATE_GROUP_CTRL(UIStep3.grp, HFSTRC(IDS_MAINWINDOW_STEP3_TITLE), HFUIC::MainWindow::UIStep3::grpRect);
    {
        CREATE_PUSHBUTTON(
            UIStep3.btnRun, HFSTRC(IDS_MAINWINDOW_STEP3_BTNRUN),
            HFUIC::MainWindow::UIStep3::btnRunRect, UIStep3.grp, HFUIC::MainWindow::ID_btnRun);
        UIStep3.btnPreviewDraw.Create(
            HFSTRC(IDS_MAINWINDOW_STEP3_PREVIEW), WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE,
            HFUIC::MainWindow::UIStep3::btnPreviewDrawRect, &UIStep3.grp, HFUIC::MainWindow::ID_btnPreviewDraw);
        UIStep3.btnPreviewDraw.SetFont(&HFUIC::Font.NORMAL_FONT);
    }

    CREATE_GROUP_CTRL(UIStep4.grp, HFSTRC(IDS_MAINWINDOW_STEP4_TITLE), HFUIC::MainWindow::UIStep4::grpRect);
    {
        CREATE_PUSHBUTTON(
            UIStep4.btnPackage, HFSTRC(IDS_MAINWINDOW_STEP4_BTNPACKAGE),
            HFUIC::MainWindow::UIStep4::btnPackageRect, UIStep4.grp, HFUIC::MainWindow::ID_btnPackage);
        CREATE_PUSHBUTTON(
            UIStep4.btnOpenFolder, HFSTRC(IDS_MAINWINDOW_STEP4_BTNOPENFOLDER),
            HFUIC::MainWindow::UIStep4::btnOpenFolderRect, UIStep4.grp, HFUIC::MainWindow::ID_btnOpenFolder);
    }

#undef CREATE_GROUP_CTRL
#undef CREATE_PUSHBUTTON
#undef CREATE_LABEL
#undef CREATE_EDIT
#undef CREATE_NUMEDIT
#undef CREATE_CHECKBUTTON

    SetFontInfoToGUI(m_fiFonts[m_iFontIndex]);
    UpdateLblPreview();

    m_ttcMain.Create(this, TTS_ALWAYSTIP);
    m_ttcMain.AddToolThatWorks(_T(""), &UIStep1.txtPak);
    m_ttcMain.AddToolThatWorks(_T(""), &UIStep2.ddlFontSelect);
    m_ttcMain.AddToolThatWorks(_T(""), &UIStep2.ddlHeaderSelect);
    m_ttcMain.Activate(TRUE);
    return 0;
}

void HFMainWindow::OnBtnBrowsePakClicked() {
    CFileDialog cfdDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST,HFSTRC(IDS_MAINWINDOW_STEP1_PAKFILTER), this);
    if (cfdDlg.DoModal() == IDOK) {
        UIStep1.txtPak.SetWindowText(cfdDlg.GetFileName());
    }
}

void HFMainWindow::OnBtnItalicClicked() {
    GetFontInfoFromGUI(m_fiFonts[m_iFontIndex]);
    UpdateLblPreview();
}

void HFMainWindow::OnBtnUnderlineClicked() {
    GetFontInfoFromGUI(m_fiFonts[m_iFontIndex]);
    UpdateLblPreview();
}

void HFMainWindow::OnBtnRunClicked() {
    m_dcDrawCentre.DrawAtOnce(m_fiFonts);
    //m_drawWnd->SetDrawCentre(&m_dcDrawCentre);
    HFBinFilesInfo info;
    info.InitializeInstance(_T("D:\\games\\TOE31\\data\\texts\\texts.pak"));
    CString sTemp;
    for (int i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        if (i != HFLC::header::HEADER_32) continue;
        size_t cuiNew = m_dcDrawCentre[i].GetUnicodeCount();
        LPUNICODEINFO auiNew = mem::GetMem<UNICODEINFO>(cuiNew);
        for (size_t j = 0; j < cuiNew; j++) {
            //auiNew[i].wcUnicode = 0;
            //auiNew[i].aiPos[0] = {0};
            m_dcDrawCentre[i].FillUNICODEINFO(j, &auiNew[j]);
        }
        info[i].Replace(auiNew, cuiNew);
        LOG.log(str::Bytes2String(info[i].m_lpHead, info[i].m_cbHead));
        sTemp.Format(_T("D:\\data\\%s"), (LPCTSTR)info[i].GetBinUID());
        info[i].CreateBinFile(sTemp);
        sTemp.Format(_T("D:\\data\\%s.txt"), (LPCTSTR)info[i].GetBinUID());
        info[i].CreateTxtFile(sTemp);
    }
}

void HFMainWindow::OnBtnPreviewDrawClicked() {
    SetBtnPreviewDrawChecked(!UIStep3.btnPreviewDraw.GetCheck());
}

void HFMainWindow::OnBtnPackageClicked() {
    CString sTemp;
    sTemp.Format(_T("Package clicked, %.8f"), 3.1415926);
    LOG.log(sTemp);
}

void HFMainWindow::OnBtnOpenFolderClicked() {
    LOG.log(_T("Open folder clicked"));
}

void HFMainWindow::OnTxtPakChange() {
    CString sResult = ValidateTxtPak();
    if (sResult.GetLength() > 0) {
        ::AfxMessageBox(sResult);
    }
}

void HFMainWindow::OnTxtPaddingChange() {
    GetFontInfoFromGUI(m_fiFonts[m_iFontIndex]);
    UpdateLblPreview();
}

void HFMainWindow::OnTxtVPositionChange() {
    GetFontInfoFromGUI(m_fiFonts[m_iFontIndex]);
    UpdateLblPreview();
}

void HFMainWindow::OnTxtBoldChange() {
    GetFontInfoFromGUI(m_fiFonts[m_iFontIndex]);
    UpdateLblPreview();
}

void HFMainWindow::OnTxtSizeChange() {
    GetFontInfoFromGUI(m_fiFonts[m_iFontIndex]);
    UpdateLblPreview();
}

void HFMainWindow::OnDdlHeaderSelectChange() {
    CString sTemp;
    UIStep2.ddlHeaderSelect.GetLBText(UIStep2.ddlHeaderSelect.GetCurSel(), sTemp);
    GetFontInfoFromGUI(m_fiFonts[m_iFontIndex]);
    m_iFontIndex = HFLC::header::LPSTR_TO_CODE(sTemp);
    SetFontInfoToGUI(m_fiFonts[m_iFontIndex]);
    UpdateLblPreview();
}

void HFMainWindow::OnDdlFontSelectChange() {
    GetFontInfoFromGUI(m_fiFonts[m_iFontIndex]);
    UpdateLblPreview();
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
    SetBtnPreviewDrawChecked(FALSE);
    return 0;
}
