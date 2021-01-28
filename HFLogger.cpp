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

    BOOL HFLogger::log(CString CONST& sMessage, COLORREF CONST& crText,
                       BOOL bBold, BOOL bItalic, BOOL bUnderline, BOOL bNewLine) {
        if (m_logWnd != NULL) {
            LOGMSG* pLogInfoStruct = new LOGMSG;
            pLogInfoStruct->sMsg = sMessage;
            pLogInfoStruct->crText = crText;
            pLogInfoStruct->bBold = bBold;
            pLogInfoStruct->bItalic = bItalic;
            pLogInfoStruct->bUnderline = bUnderline;
            pLogInfoStruct->bNewLine = bNewLine;
            m_logWnd->SendMessage(UIConst::WindowMessage::LOG_SHOW_TEXT, NULL, (LPARAM)pLogInfoStruct);
            delete pLogInfoStruct;
        }
        return m_logWnd != NULL;
    }

    BOOL HFLogger::SaveLog(LPCTSTR szRichTextFilename) {
        if (m_logWnd != NULL) {
            CString sRichTextFilename(szRichTextFilename);
            m_logWnd->SendMessage(UIConst::WindowMessage::LOG_SAVE_TEXT, NULL, (LPARAM)&sRichTextFilename);
        }
        return m_logWnd != NULL;
    }

    BOOL HFLogger::__log_winapi_exception(
        LPCTSTR szFileName,
        int iLineNo,
        LPCTSTR szFuncName,
        LPCTSTR szAPIName,
        DWORD dwLastError,
        INT64 iReturnValue,
        LPCTSTR szComment) {
        if (m_logWnd != NULL) {
            CString sMsg(_T("WinAPI error: "));
            log(sMsg, UIConst::Color::RED, TRUE, FALSE, FALSE, FALSE);
            sMsg.Format(
                _T("%s. \"%s\" WinAPI returned %lld with lastError of %lu, from \"%s\" at line %d in file %s"),
                szComment, szAPIName, iReturnValue, dwLastError, szFuncName, iLineNo, szFileName);
            log(sMsg, UIConst::Color::RED);
        }
        return m_logWnd != NULL;
    }

    BOOL HFLogger::__log_user_exception(
        LPCTSTR szFileName,
        int iLineNo,
        LPCTSTR szFuncName,
        LPCTSTR szDescription) {
        if (m_logWnd != NULL) {
            CString sMsg(_T("User error: "));
            log(sMsg, UIConst::Color::RED, TRUE, FALSE, FALSE, FALSE);
            sMsg.Format(
                _T("%s, from \"%s\" at line %d in file %s"),
                szDescription, szFuncName, iLineNo, szFileName);
            log(sMsg, UIConst::Color::RED);
        }
        return m_logWnd != NULL;
    }

}