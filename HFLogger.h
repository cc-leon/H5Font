#pragma once
#define LOG logger::HFLogger::get()
#define LOGAPI(api,value,cmnt) \
    logger::HFLogger::get().__log_winapi_exception(_T(__FILE__), __LINE__, _T(__FUNCTION__), _T(api), ::GetLastError(), (INT64)(value), _T(cmnt))
#define LOGUSR(desc) \
    logger::HFLogger::get().(_T(__FILE__), __LINE__, _T(__FUNCTION__), _T(desc))

namespace logger {
    class HFLogger {
    public:
        VOID Initialize(CWnd* CONST pLogWnd);

        static HFLogger& get();

        BOOL log(CString CONST& sMessage,
                 COLORREF CONST& crText = RGB(255, 255, 255),
                 BOOL bBold = FALSE,
                 BOOL bItalic = FALSE,
                 BOOL bUnderline = FALSE,
                 BOOL bNewLine = TRUE);

        BOOL SaveLog(LPCTSTR szRichTextFilename);

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

    protected:
        HFLogger();
        static CWnd* m_logWnd;
    };
}