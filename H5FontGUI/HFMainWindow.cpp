#include "pch.h"

#include "HFMainWindow.h"
#include "HFDrawWindow.h"

UINT RunThreadFunc(LPVOID lpParam) {
    HFMainWindow* main = (HFMainWindow*)lpParam;
    main->m_bfFileCentre.InitializeInstance(main->ThreadData.pakpath);

    int i = 0, prev_i;
    do {
        prev_i = i;
        if (main->ThreadData.cancelled) {
            ::AfxEndThread(7);
        }
        main->m_dcDrawCentre.DrawOneByOne(&main->m_fiFonts[i], i);
    } while (i > prev_i);

    main->m_drawWnd->SetDrawCentre(&main->m_dcDrawCentre);
    LOG.log(_T(""));

    main->ThreadData.dialog->PostMessage(WM_COMMAND, IDOK);

    return 0;
}

UINT PackThreadFunc(LPVOID lpParam) {
    HFMainWindow* main = (HFMainWindow*)lpParam;

    int i = 0, prev_i;
    do {
        prev_i = i;
        if (main->ThreadData.cancelled) {
            ::AfxEndThread(7);
        }
        main->m_dcDrawCentre.SaveOneByOne(i);
    } while (i > prev_i);

    if (main->ThreadData.cancelled) {
        ::AfxEndThread(7);
    }
    main->m_bfFileCentre.SyncFromDrawCentre(&main->m_dcDrawCentre);

    i = 0;
    do {
        prev_i = i;
        if (main->ThreadData.cancelled) {
            ::AfxEndThread(7);
        }
        main->m_bfFileCentre.SaveOneByOne(i);
    } while (i > prev_i);

    if (main->ThreadData.cancelled) {
        ::AfxEndThread(7);
    }

    LOG.log(_T(""));
    file::ZipFile(main->ThreadData.pakpath, HFFC::pak::TEMP_FOLDER);

    if (main->ThreadData.cancelled) {
        ::AfxEndThread(7);
    }

    if (main->ThreadData.deltemp) {
        LOG.log(_T(""));
        file::ClearTempFile();
    }
    LOG.log(_T(""));

    main->ThreadData.dialog->PostMessage(WM_COMMAND, IDOK);

    return 0;
}


HFMainWindow::HFMainWindow() 
    : m_logWnd(NULL), m_drawWnd(NULL), m_mnMain(), m_iFontIndex(0), ThreadData{NULL, _T(""), FALSE},
    m_fRunned(FALSE) {

    if (file::FileExists(HFUIC::MainWindow::szPresetFilename)) {
        SIZE_T cbToPick = sizeof(FONTINFO) * HFLC::header::HEADER_COUNT;
        SIZE_T cbPicked = file::PickFromFile(HFUIC::MainWindow::szPresetFilename, NULL);
        if (cbPicked != cbToPick) {
            goto NORM;
        }
        else {
            LPFONTINFO pfiTemp = mem::GetMem<FONTINFO>(cbPicked / sizeof(FONTINFO));
            file::PickFromFile(HFUIC::MainWindow::szPresetFilename, pfiTemp);
            ::CopyMemory(m_fiFonts, pfiTemp, cbPicked);
            mem::FreeMem(pfiTemp);
            return;
        }
    }
NORM:
    for (int i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        ::StringCchCopy(m_fiFonts[i].szFacenam, LF_FACESIZE, _T("ºÚÌå"));
        m_fiFonts[i].nPadding = 0;
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
    sTemp = fi.szFacenam;
    m_ttcMain.ChangeTips(sTemp.GetBuffer(), &UIStep2.ddlFontSelect);
    sTemp.Format(_T("%d"), fi.nPadding);
    UIStep2.txtPadding.SetWindowText(sTemp);
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
    ON_EN_KILLFOCUS(HFUIC::MainWindow::ID_txtBold, &HFMainWindow::OnTxtBoldChange)
    ON_EN_KILLFOCUS(HFUIC::MainWindow::ID_txtSize, &HFMainWindow::OnTxtSizeChange)
    ON_CBN_SELENDOK(HFUIC::MainWindow::ID_ddlFontSelect, &HFMainWindow::OnDdlFontSelectChange)
    ON_CBN_SELENDOK(HFUIC::MainWindow::ID_ddlHeaderSelect, &HFMainWindow::OnDdlHeaderSelectChange)

    // Menu messages
    ON_COMMAND(ID_FILE_SAVE_PRESET, &HFMainWindow::OnFileSavePreset)
    ON_COMMAND(ID_FILE_LOAD_PRESET, &HFMainWindow::OnFileLoadPreset)
    ON_COMMAND(IDM_WINDOWS_LOG, &HFMainWindow::OnWindowsLog)
    ON_COMMAND(IDM_WINDOWS_CLEAR_LOG, &HFMainWindow::OnWindowsLogClear)
    ON_COMMAND(IDM_WINDOWS_SAVE_LOG, &HFMainWindow::OnWindowsLogSave)
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
    SIZE_T cbToDump = sizeof(FONTINFO) * HFLC::header::HEADER_COUNT;
    SIZE_T cbDumped = file::DumpInFile(HFUIC::MainWindow::szPresetFilename, m_fiFonts, cbToDump);

    m_logWnd->DestroyWindow();
    m_drawWnd->DestroyWindow();
    CFrameWnd::OnClose();
}

int HFMainWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MAIN_ICON));
    SetIcon(hIcon, FALSE);

    SetFont(&HFUIC::Font.NORMAL_FONT);

    m_ttcMain.Create(this, TTS_ALWAYSTIP);

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

    m_ttcMain.AddToolThatWorks(_T(""), &UIStep1.txtPak);
    m_ttcMain.AddToolThatWorks(_T(""), &UIStep2.ddlFontSelect);
    m_ttcMain.Activate(TRUE);

    SetFontInfoToGUI(m_fiFonts[m_iFontIndex]);
    UpdateLblPreview();

    return 0;
}

void HFMainWindow::OnBtnBrowsePakClicked() {
    CFileDialog cfdDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST,HFSTRC(IDS_MAINWINDOW_STEP1_PAKFILTER), this);
    if (cfdDlg.DoModal() == IDOK) {
        UIStep1.txtPak.SetWindowText(cfdDlg.GetPathName());
    }
    OnTxtPakChange();
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
    CString sValidate;
    sValidate = ValidateTxtPak();
    if (sValidate.GetLength() > 0) {
        ::AfxMessageBox(sValidate, MB_OK | MB_ICONERROR);
        return;
    }

    UIStep1.txtPak.GetWindowText(ThreadData.pakpath);
    SetBtnPreviewDrawChecked(FALSE);

    HFProcessDlg dlg(this);
    ThreadData.dialog = &dlg;
    ThreadData.cancelled = FALSE;
    CWinThread *pThread = ::AfxBeginThread(RunThreadFunc, this);
    CString sTemp;
    sTemp.Format(
        _T("%s\n%s"),
        (LPCTSTR)HFSTRC(IDS_MSG_RUN_TASK_IS_GOING1),
        (LPCTSTR)HFSTRC(IDS_MSG_RUN_TASK_IS_GOING2));

    INT_PTR dd = dlg.DoModal(
        sTemp,
        HFSTRC(IDS_MSG_RUN_TITLE),
        HFSTRC(IDS_MSG_RUN_TAKS_CANCEL_WARNING));

    if (dd == IDCANCEL) {
        m_fRunned = FALSE;
        ThreadData.cancelled = TRUE;
        ::MessageBox(
            *this,
            HFSTRC(IDS_MSG_TASK_CANCELLED),
            HFSTRC(IDS_MSG_TASK_CANCELLED),
            MB_OK | MB_ICONINFORMATION);
    }
    else {
        m_fRunned = TRUE;
        ::MessageBox(
            *this,
            HFSTRC(IDS_MSG_TASK_FINISHED),
            HFSTRC(IDS_MSG_TASK_FINISHED),
            MB_OK | MB_ICONINFORMATION);
    }
    return;
}

void HFMainWindow::OnBtnPreviewDrawClicked() {
    if (!m_fRunned) {
        ::AfxMessageBox(HFSTRC(IDS_MSG_STEP3_NOT_RUN), MB_OK | MB_ICONSTOP);
        return;
    }
    SetBtnPreviewDrawChecked(!UIStep3.btnPreviewDraw.GetCheck());
}

void HFMainWindow::OnBtnPackageClicked() {
    if (!m_fRunned) {
        ::AfxMessageBox(HFSTRC(IDS_MSG_STEP3_NOT_RUN), MB_OK | MB_ICONSTOP);
        return;
    }

    CFileDialog cfdDlg(
        FALSE, _T("pak"), _T("berein's font"), 
        OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_SHOWHELP,
        HFSTRC(IDS_MAINWINDOW_STEP1_PAKFILTER), this);

    if (cfdDlg.DoModal() == IDCANCEL) {
        ::AfxMessageBox(HFSTRC(IDS_MSG_SAVE_FILE_CANCELLED), MB_OK | MB_ICONWARNING);
        return;
    }

    CString sTemp;
    sTemp.Format(
        _T("%s\n%s"),
        (LPCTSTR)HFSTRC(IDS_MSG_DELETE_TEMP1),
        (LPCTSTR)HFSTRC(IDS_MSG_DELETE_TEMP2));
    CString sTemp2;
    sTemp2.Format(sTemp, HFFC::pak::TEMP_FOLDER);
    if (::AfxMessageBox(sTemp2, MB_YESNO | MB_ICONQUESTION) == IDYES) {
        ThreadData.deltemp = TRUE;
    }
    else {
        ThreadData.deltemp = FALSE;
    }

    ThreadData.pakpath = cfdDlg.GetPathName();
    ThreadData.cancelled = FALSE;

    HFProcessDlg dlg(this);
    ThreadData.dialog = &dlg;
    ThreadData.cancelled = FALSE;
    CWinThread* pThread = ::AfxBeginThread(PackThreadFunc, this);
    sTemp.Format(
        _T("%s\n%s"),
        (LPCTSTR)HFSTRC(IDS_MSG_PACK_TASK_IS_GOING1),
        (LPCTSTR)HFSTRC(IDS_MSG_PACK_TASK_IS_GOING2));

    INT_PTR dd = dlg.DoModal(
        sTemp,
        HFSTRC(IDS_MSG_RUN_TITLE),
        HFSTRC(IDS_MSG_RUN_TAKS_CANCEL_WARNING));

    if (dd == IDCANCEL) {
        ThreadData.cancelled = TRUE;
        ::MessageBox(
            *this,
            HFSTRC(IDS_MSG_TASK_CANCELLED),
            HFSTRC(IDS_MSG_TASK_CANCELLED),
            MB_OK | MB_ICONINFORMATION);
    }
    else {
        ::MessageBox(
            *this,
            HFSTRC(IDS_MSG_TASK_FINISHED),
            HFSTRC(IDS_MSG_TASK_FINISHED),
            MB_OK | MB_ICONINFORMATION);
    }
    return;
}

void HFMainWindow::OnBtnOpenFolderClicked() {
    if (!m_fRunned || !file::FileExists(ThreadData.pakpath)) {
        ::AfxMessageBox(HFSTRC(IDS_MSG_PAK_FILE_NOT_FOUND), MB_OK | MB_ICONWARNING);
        return;
    }
    CString sTemp(ThreadData.pakpath);
    sTemp.Truncate(sTemp.ReverseFind(_T('\\')));
    ::ShellExecute(NULL, _T("explore"), sTemp, NULL, NULL, SW_SHOW);
}

void HFMainWindow::OnTxtPakChange() {
    CString sResult = ValidateTxtPak();
    if (sResult.GetLength() > 0) {
        ::AfxMessageBox(sResult);
    }
    CString sTemp;
    UIStep1.txtPak.GetWindowText(sTemp);
    m_ttcMain.ChangeTips(sTemp.GetBuffer(), &UIStep1.txtPak);
}

void HFMainWindow::OnTxtPaddingChange() {
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
    CString sTemp;
    if (UIStep2.ddlFontSelect.GetCurSel() != LB_ERR) {
        UIStep2.ddlFontSelect.GetLBText(UIStep2.ddlFontSelect.GetCurSel(), sTemp);
    }

    m_ttcMain.ChangeTips(sTemp.GetBuffer(), &UIStep2.ddlFontSelect);
}

void HFMainWindow::OnFileSavePreset() {
    CFileDialog cfdDlg(
        FALSE, _T("hfbin"), _T("default"),
        OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_SHOWHELP,
        HFSTRC(IDS_MAINWINDOW_HFBINFILTER), this);

    if (cfdDlg.DoModal() == IDOK) {
        CString sHFBinName = cfdDlg.GetPathName();
        CString sTemp;

        SIZE_T cbToDump = sizeof(FONTINFO) * HFLC::header::HEADER_COUNT;
        SIZE_T cbDumped = file::DumpInFile(sHFBinName, m_fiFonts, cbToDump);

        if (cbDumped != cbToDump) {
            sTemp.Format(HFSTRC(IDS_MAINWINDOW_SAVE_PRESET_FAILURE), sHFBinName);
            ::AfxMessageBox(sTemp, MB_OK | MB_ICONERROR);
            ::DeleteFile(sHFBinName);
        }
        else {
            sTemp.Format(HFSTRC(IDS_MAINWINDOW_SAVE_PRESET_SUCCESS), sHFBinName);
            ::AfxMessageBox(sTemp, MB_OK | MB_ICONINFORMATION);
        }
    }
}

void HFMainWindow::OnFileLoadPreset() {
    CFileDialog cfdDlg(
        TRUE, NULL, NULL, OFN_FILEMUSTEXIST,
        HFSTRC(IDS_MAINWINDOW_HFBINFILTER), this);

    if (cfdDlg.DoModal() == IDOK) {
        CString sHFBinName = cfdDlg.GetPathName();
        CString sTemp;

        SIZE_T cbToPick = sizeof(FONTINFO) * HFLC::header::HEADER_COUNT;
        SIZE_T cbPicked = file::PickFromFile(sHFBinName, NULL);
        if (cbPicked != cbToPick) {
            sTemp.Format(HFSTRC(IDS_MAINWINDOW_LOAD_PRESET_FAILURE), sHFBinName);
            ::AfxMessageBox(sTemp, MB_OK | MB_ICONERROR);
        }
        else {
            LPFONTINFO pfiTemp = mem::GetMem<FONTINFO>(cbPicked / sizeof(FONTINFO));
            file::PickFromFile(sHFBinName, pfiTemp);
            ::CopyMemory(m_fiFonts, pfiTemp, cbPicked);
            mem::FreeMem(pfiTemp);
            SetFontInfoToGUI(m_fiFonts[m_iFontIndex]);
            sTemp.Format(HFSTRC(IDS_MAINWINDOW_LOAD_PRESET_SUCCESS), sHFBinName);
            ::AfxMessageBox(sTemp, MB_OK | MB_ICONINFORMATION);
        }
    }
}

void HFMainWindow::OnWindowsLog() {
    SetMenuWindowsLogChecked(!GetMenuChecked(IDM_WINDOWS_LOG));
}


void HFMainWindow::OnWindowsLogSave() {
    CFileDialog cfdDlg(
        FALSE, _T("rtf"), NULL,
        OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_SHOWHELP,
        HFSTRC(IDS_MAINWINDOW_RTF_FILTER), this);

    if (cfdDlg.DoModal() == IDOK) {
        LOG.SaveLog(cfdDlg.GetPathName());
    }

}

void HFMainWindow::OnWindowsLogClear() {
    LOG.ClearLog();
}

void HFMainWindow::OnHelpReadme() {
    if (!file::FileExists(HFUIC::MainWindow::szHelpFilename)) {
        CString sTemp;
        sTemp.Format(HFSTRC(IDS_MSG_HELP_FILE_MISSING), HFUIC::MainWindow::szHelpFilename);
        ::AfxMessageBox(sTemp, MB_OK | MB_ICONERROR);
    }
    ::ShellExecute(NULL, _T("open"), HFUIC::MainWindow::szHelpFilename, NULL, NULL, SW_SHOW);
}

void HFMainWindow::OnHelpOnline() {
    ::ShellExecute(NULL, _T("open"), _T("https://github.com/cc-leon/H5Font"), NULL, NULL, SW_SHOW);
}

void HFMainWindow::OnHelpAbout() {
    CDialogEx abtDlg(IDD_ABOUTBOX, this);
    abtDlg.DoModal();
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
