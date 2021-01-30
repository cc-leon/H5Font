#pragma once
#define LOG logger::DefaultLogger
#define LOGAPI(api,value,cmnt) \
    LOG.__log_winapi_exception(_T(__FILE__), __LINE__, _T(__FUNCTION__), _T(api), ::GetLastError(), (INT64)(value), _T(cmnt))
#define LOGUSR(desc) \
    LOG.(_T(__FILE__), __LINE__, _T(__FUNCTION__), _T(desc))

namespace logger {
    class HFLogger {
    public:
        static COLORREF CONST RED = RGB(255, 0, 0);

        HFLogger();
        ~HFLogger();
        HFLogger(HFLogger CONST&) = delete;
        HFLogger(HFLogger &&) = delete;
        VOID operator = (HFLogger CONST&) = delete;
        VOID operator = (HFLogger &&) = delete;

        VOID Initialize(CWnd* CONST pLogWnd, LPCTSTR szLogFilename=NULL);

        BOOL log(CString CONST& sMessage,
                 COLORREF CONST& crText = RGB(255, 255, 255),
                 BOOL bBold = FALSE,
                 BOOL bItalic = FALSE,
                 BOOL bUnderline = FALSE,
                 BOOL bNewLine = TRUE);

        BOOL __log_winapi_exception(
            LPCTSTR szFileName,
            int iLineNo,
            LPCTSTR szFuncName,
            LPCTSTR szAPIName,
            DWORD dwLastError,
            INT64 iReturnValue,
            LPCTSTR szComment);

        BOOL __log_user_exception(
            LPCTSTR szFileName,
            int ilineNo,
            LPCTSTR szFuncName,
            LPCTSTR szDescription);

        BOOL SaveLog(LPCTSTR szRichTextFilename);

    protected:
        CWnd* m_logWnd;
        CFile* m_hFile;
    };

    extern HFLogger DefaultLogger;
}