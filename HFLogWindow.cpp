// HFLogWindow.cpp : implementation file
//

#include "pch.h"
#include "HFLogWindow.h"
#include "HFLogLib.h"

// HFLogWindow
HFLogWindow::HFLogWindow() :m_txtLog(NULL), m_parentWnd(NULL) {}

HFLogWindow::~HFLogWindow() {}


BOOL HFLogWindow::PreCreateWindow(CREATESTRUCT& cs) {
    if (!CFrameWnd::PreCreateWindow(cs)) {
        return FALSE;
    }

    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.cx = UIConst::LogWindow::Size.Width();
    cs.cy = UIConst::LogWindow::Size.Height();
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
    ON_MESSAGE(UIConst::WindowMessage::LOG_SHOW_TEXT, &HFLogWindow::OnLogmsg)
    ON_MESSAGE(UIConst::WindowMessage::LOG_SAVE_TEXT, &HFLogWindow::OnLogsave)
END_MESSAGE_MAP()


// HFLogWindow message handlers

void HFLogWindow::OnClose() {
    ShowWindow(SW_HIDE);
}


int HFLogWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    CFont newFont;
    newFont.CreateFont(
        16,                            // Height
        16,                             // Width
        0,                             // Escapement
        0,                             // OrientationME
        FW_REGULAR,                       // Weight
        FALSE,                         // Italic
        FALSE,                          // Underline
        0,                             // StrikeOut
        ANSI_CHARSET,                  // CharSet
        OUT_DEVICE_PRECIS,            // OutPrecision
        CLIP_DEFAULT_PRECIS,           // ClipPrecision
        CLEARTYPE_QUALITY,               // Quality
        FIXED_PITCH | FF_ROMAN,      // PitchAndFamily
        _T("Consolas"));      // Facename

    m_txtLog = new HFRichTextBox;
    m_txtLog->Create(
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL 
        | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE | ES_READONLY,
        CRect(0, 0, lpCreateStruct->cx, lpCreateStruct->cy),
        this, ID_txtLog);
    m_txtLog->SetBackgroundColor(FALSE, UIConst::Color::BLACK);
    m_txtLog->SetFont(&newFont);
    m_txtLog->SetUndoLimit(0);
    return 0;
}


void HFLogWindow::OnSize(UINT nType, int cx, int cy) {
    CFrameWnd::OnSize(nType, cx, cy);
    m_txtLog->SetWindowPos(&wndTop, 0, 0, cx, cy, NULL);
}



afx_msg LRESULT HFLogWindow::OnLogmsg(WPARAM wParam, LPARAM lParam) {
    logger::LOGMSG* pLogInfoStruct = (logger::LOGMSG*)lParam;
    long lTxtBoxLen = m_txtLog->GetTextLength();
    m_txtLog->SetSel(lTxtBoxLen, lTxtBoxLen);
    CHARFORMAT newFormat;
    ::ZeroMemory(&newFormat, sizeof(CHARFORMAT));
    newFormat.cbSize = sizeof(newFormat);
    newFormat.dwMask = CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE;
    if (pLogInfoStruct->bBold) {
        newFormat.dwEffects |= CFE_BOLD;
    }
    if (pLogInfoStruct->bItalic) {
        newFormat.dwEffects |= CFE_ITALIC;
    }
    if (pLogInfoStruct->bUnderline) {
        newFormat.dwEffects |= CFE_UNDERLINE;
    }
    newFormat.crTextColor = pLogInfoStruct->crText;
    m_txtLog->SetSelectionCharFormat(newFormat);

    if (pLogInfoStruct->bNewLine == TRUE) {
        pLogInfoStruct->sMsg += CString(_T('\n'));
    }
    m_txtLog->ReplaceSel(pLogInfoStruct->sMsg, 0);
    m_txtLog->LineScroll(1);

    return 0;
}

static DWORD CALLBACK HFTxtLogStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb) {
    CFile* pFile = (CFile*)dwCookie;

    pFile->Write(pbBuff, cb);
    *pcb = cb;

    return 0;
}

afx_msg LRESULT HFLogWindow::OnLogsave(WPARAM wParam, LPARAM lParam) {
    CFile cFile(*(CString*)(lParam), CFile::modeCreate | CFile::modeWrite);
    EDITSTREAM es;

    es.dwCookie = (DWORD)&cFile;
    es.pfnCallback = HFTxtLogStreamOutCallback;
    m_txtLog->StreamOut(SF_RTF, es);

    return 0;
}
