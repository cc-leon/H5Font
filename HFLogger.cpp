#include "pch.h"
#include "HFLogger.h"
#include "HFLogLib.h"

namespace logger {
    CWnd * HFLogger::m_logWnd = NULL;

    VOID HFLogger::Initialize(CWnd* CONST pLogWnd) {
        m_logWnd = pLogWnd;
    }

    HFLogger::HFLogger() {
    }

    HFLogger& HFLogger::get() {
        static HFLogger newInstance;
        return newInstance;
    }

    VOID HFLogger::log(LPCTSTR szMessage, COLORREF CONST& crText,
                       BOOL bBold, BOOL bItalic, BOOL bUnderline, BOOL bNewLine) {
        LOGMSG* pLogInfoStruct = new LOGMSG;
        pLogInfoStruct->SetMsg(szMessage);
        pLogInfoStruct->crText = crText;
        pLogInfoStruct->bBold = bBold;
        pLogInfoStruct->bItalic = bItalic;
        pLogInfoStruct->bUnderline = bUnderline;
        pLogInfoStruct->bNewLine = bNewLine;
        m_logWnd->SendMessage(logger::MESSAGE_ID, NULL, (LPARAM)pLogInfoStruct);
        delete pLogInfoStruct;
    }
}