#pragma once
#include "HFLogWindow.h"
#include "HFDrawWindow.h"
#include "HFGroupBox.h"
#include "HFHeaderDropDownList.h"
#include "HFFontDropDownList.h"
#include "HFToolTipCtrl.h"

class HFMainWindow : public CFrameWnd {

protected:
    HFLogWindow * m_logWnd;
    HFDrawWindow* m_drawWnd;
    CMenu m_mnMain;
    HFToolTipCtrl m_ttcMain;

    struct {
        HFGroupBox grp;
        CEdit txtPak; CButton btnBrowsePak; CStatic lblPak;
    } UIStep1;

    struct {
        HFGroupBox grp;
        CStatic lblHeaderSelect; HFHeaderDropDownList ddlHeaderSelect; CStatic lblSplitter;
        CStatic lblFontSelect; HFFontDropDownList ddlFontSelect;
        CStatic lblSize; CEdit txtSize; CStatic lblBold; CEdit txtBold;
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
    VOID SetMenuWindowsDrawChecked(BOOL bChecked);

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
    afx_msg void OnTxtBoldChange();
    afx_msg void OnTxtSizeChange();
    afx_msg void OnDdlHeaderSelectChange();
    afx_msg void OnDdlFontSelectChange();

    // Menus messages
    afx_msg void OnWindowsLog();
    afx_msg void OnHelpReadme();
    afx_msg void OnHelpOnline();
    afx_msg void OnHelpAbout();
    afx_msg void OnLanguageEnglish();
    afx_msg void OnLanguageChinese();

    // Custom messges
    afx_msg LRESULT OnWindowlog(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnWindowdraw(WPARAM wParam, LPARAM lParam);

};
