#include "pch.h"
#include "HFProcessDlg.h"

TCHAR CONST HFProcessDlg::WAITING[8] = {_T('-'), _T('\0'), _T('\\'), _T('\0'), _T('|'), _T('\0'), _T('/'), _T('\0')};

HFProcessDlg::HFProcessDlg(CWnd* pParent)
    : CDialogEx(IDD_PROCESSING, pParent), m_iIndex(0){}

HFProcessDlg::~HFProcessDlg()
{}

void HFProcessDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
}

INT_PTR HFProcessDlg::DoModal(LPCTSTR szPrompt, LPCTSTR szCaption, LPCTSTR szCancelWarning) {
    m_sCaption = szCaption;
    m_sPrompt = szPrompt;
    m_sCancelWarning = szCancelWarning;
    return CDialogEx::DoModal();
}

BOOL HFProcessDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
    GetDlgItem(IDC_PROCESSING_STATIC_BAR)->SetFont(&HFUIC::Font.LOG_FONT);
    GetDlgItem(IDC_PROCESSING_STATIC_BAR)->SetFont(&HFUIC::Font.NORMAL_FONT);
    GetDlgItem(IDC_PROCESSING_STATIC_INFO)->SetWindowText(m_sPrompt);
    GetDlgItem(IDC_PROCESSING_STATIC_BAR)->SetWindowText(_T(""));
    SetWindowText(m_sCaption);
    SetTimer(IDD_PROCESSING_TIMER, 100, NULL);
    return TRUE;
}

void HFProcessDlg::OnCancel() {
    if (::MessageBox(*this, m_sCancelWarning, m_sCaption, MB_OKCANCEL | MB_ICONQUESTION) == IDOK) {
        CDialogEx::OnCancel();
    }
}


BEGIN_MESSAGE_MAP(HFProcessDlg, CDialogEx)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// HFProgressDlg message handlers

void HFProcessDlg::OnTimer(UINT_PTR nIDEvent) {
    CDialogEx::OnTimer(nIDEvent);
    if (nIDEvent == IDD_PROCESSING_TIMER) {
        m_iIndex += 2;
        if (m_iIndex >= 8) {
            m_iIndex = 0;
        }
        GetDlgItem(IDC_PROCESSING_STATIC_BAR)->SetWindowText(&WAITING[m_iIndex]);
    }
}


