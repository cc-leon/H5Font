#pragma once
#include "HFLogWindow.h"
#include "HFDrawWindow.h"
#include "HFGroupBox.h"
#include "HFHeaderDropDownList.h"
#include "HFFontDropDownList.h"
#include "HFToolTipCtrl.h"
#include "HFProcessDlg.h"
#include "../H5FontCore/HFDrawDCsCentre.h"
#include "../H5FontFileIO/HFBinFilesInfo.h"

class HFMainWindow : public CFrameWnd {

protected:
    friend UINT RunThreadFunc(LPVOID lpParam);
    friend UINT PackThreadFunc(LPVOID lpParam);

    HFLogWindow * m_logWnd;
    HFDrawWindow* m_drawWnd;
    CMenu m_mnMain;
    HFToolTipCtrl m_ttcMain;
    BOOL m_fRunned;

    // Logicpart
    FONTINFO m_fiFonts[HFLC::header::HEADER_COUNT];
    int m_iFontIndex;
    HFDrawDCsCentre m_dcDrawCentre;
    HFBinFilesInfo m_bfFileCentre;

    struct {
        HFProcessDlg* dialog;
        CString pakpath;
        BOOL cancelled;
        BOOL deltemp;
    } ThreadData;

    struct {
        HFGroupBox grp;
        CEdit txtPak; CButton btnBrowsePak; CStatic lblPak;
    } UIStep1;

    struct {
        HFGroupBox grp;
        CStatic lblHeaderSelect; HFHeaderDropDownList ddlHeaderSelect; CStatic lblSplitter;
        CStatic lblFontSelect; HFFontDropDownList ddlFontSelect; CStatic lblSize; CEdit txtSize;
        CStatic lblPadding; CEdit txtPadding; CStatic lblBold; CEdit txtBold;
        CButton btnItalic; CButton btnUnderline; CStatic lblPreview;
    } UIStep2;

    struct {
        HFGroupBox grp; CButton btnRun; CButton btnPreviewDraw;
    } UIStep3;

    struct {
        HFGroupBox grp; CButton btnPackage; CButton btnOpenFolder;
    } UIStep4;

    BOOL GetMenuChecked(UINT uiMenuID);
    VOID SetMenuWindowsLogChecked(BOOL bChecked);
    VOID SetBtnPreviewDrawChecked(BOOL bChecked);
    VOID SetFontInfoToGUI(FONTINFO CONST& fi);
    VOID GetFontInfoFromGUI(FONTINFO & fi);
    VOID UpdateLblPreview();
    CString ValidateTxtPak();

public:
    HFMainWindow();
    virtual ~HFMainWindow();
    CWnd * GetLogWnd();
    BOOL CreateHFMainWindow();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    DECLARE_MESSAGE_MAP()
    afx_msg void OnClose();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    // Button message
    afx_msg void OnBtnBrowsePakClicked();
    afx_msg void OnBtnItalicClicked();
    afx_msg void OnBtnUnderlineClicked();
    afx_msg void OnBtnRunClicked();
    afx_msg void OnBtnPreviewDrawClicked();
    afx_msg void OnBtnPackageClicked();
    afx_msg void OnBtnOpenFolderClicked();

    // Text change message
    afx_msg void OnTxtPakChange();
    afx_msg void OnTxtPaddingChange();
    afx_msg void OnTxtBoldChange();
    afx_msg void OnTxtSizeChange();
    afx_msg void OnDdlHeaderSelectChange();
    afx_msg void OnDdlFontSelectChange();

    // Menus messages
    afx_msg void OnFileSavePreset();
    afx_msg void OnFileLoadPreset();
    afx_msg void OnWindowsLog();
    afx_msg void OnWindowsLogClear();
    afx_msg void OnWindowsLogSave();
    afx_msg void OnHelpReadme();
    afx_msg void OnHelpOnline();
    afx_msg void OnHelpAbout();
    afx_msg void OnLanguageEnglish();
    afx_msg void OnLanguageChinese();

    // Custom messges
    afx_msg LRESULT OnWindowlog(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnWindowdraw(WPARAM wParam, LPARAM lParam);

};
