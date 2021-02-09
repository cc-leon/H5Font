#pragma once

//IDC_PROCESSING_STATIC_INFO
//IDC_PROCESSING_STATIC_BAR

class HFProcessDlg : public CDialogEx {
protected:
    static TCHAR CONST WAITING[8];
    size_t m_iIndex;
    CString m_sPrompt;
    CString m_sCaption;
    CString m_sCancelWarning;

public:
    HFProcessDlg(CWnd* pParent = nullptr);   // standard constructor
    virtual ~HFProcessDlg();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual INT_PTR DoModal(LPCTSTR szPrompt, LPCTSTR szCaption, LPCTSTR szCancelWarning);
    virtual BOOL OnInitDialog();
    virtual void OnCancel();

    enum {
        IDD_PROCESSING_TIMER = IDD_PROCESSING + 0x100,
    };
// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_PROCESSING};
#endif

    DECLARE_MESSAGE_MAP()
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
