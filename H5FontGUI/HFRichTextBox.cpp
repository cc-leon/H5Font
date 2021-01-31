#include "pch.h"
#include "HFRichTextBox.h"
#include "../H5FontLogger/HFLogLib.h"

HFRichTextBox::HFRichTextBox(): CRichEditCtrl(){}

HFRichTextBox::~HFRichTextBox() {}

BOOL HFRichTextBox::CreateHFRichTextBox(CWnd* pParentWnd, CRect CONST& rect, UINT nID) {
    return Create(
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL
        | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE | ES_READONLY,
        rect, pParentWnd, nID);
}

BEGIN_MESSAGE_MAP(HFRichTextBox, CRichEditCtrl)
    //ON_CONTROL_REFLECT(EN_CHANGE, &HFRichTextBox::OnEnChange)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_MESSAGE(logger::LOG_SHOW_TEXT, &HFRichTextBox::OnLogmsg)
    ON_MESSAGE(logger::LOG_SAVE_TEXT, &HFRichTextBox::OnLogsave)
END_MESSAGE_MAP()

int HFRichTextBox::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    SetBackgroundColor(FALSE, UIC::Color::BLACK);
    SetFont(&UIC::Font.LOG_FONT);
    SetUndoLimit(0);

    //SetEventMask(GetEventMask() | ENM_REQUESTRESIZE);
    return 0;
}

void HFRichTextBox::OnDestroy() {
    LOG.SaveLog(UIC::LogWindow::szLogFilename);
    CRichEditCtrl::OnDestroy();
}

afx_msg LRESULT HFRichTextBox::OnLogmsg(WPARAM wParam, LPARAM lParam) {
    logger::LOGMSG* pLogInfoStruct = (logger::LOGMSG*)lParam;
    long lTxtBoxLen = GetTextLength();
    SetSel(lTxtBoxLen, lTxtBoxLen);
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
    SetSelectionCharFormat(newFormat);

    if (pLogInfoStruct->bNewLine == TRUE) {
        pLogInfoStruct->sMsg += CString(_T('\n'));
    }
    ReplaceSel(pLogInfoStruct->sMsg, 0);
    LineScroll(1);

    return 0;
}

static DWORD CALLBACK HFTxtLogStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb) {
    CFile* pFile = (CFile*)dwCookie;

    pFile->Write(pbBuff, cb);
    *pcb = cb;

    return 0;
}

afx_msg LRESULT HFRichTextBox::OnLogsave(WPARAM wParam, LPARAM lParam) {
    CFile cFile(*(CString*)(lParam), CFile::modeCreate | CFile::modeWrite);
    EDITSTREAM es;

    es.dwCookie = (DWORD)&cFile;
    es.pfnCallback = HFTxtLogStreamOutCallback;
    StreamOut(SF_RTF, es);

    return 0;
}
