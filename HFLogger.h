#pragma once
#define LOG logger::HFLogger::get()

namespace logger {
    class HFLogger {
    public:
        VOID Initialize(CWnd* CONST pLogWnd);
        static HFLogger& get();
        VOID log(LPCTSTR szMessage,
                 COLORREF CONST& crText = RGB(255, 255, 255),
                 BOOL bBold = FALSE,
                 BOOL bItalic = FALSE,
                 BOOL bUnderline = FALSE,
                 BOOL bNewLine = TRUE);

    protected:
        HFLogger();
        static CWnd* m_logWnd;
    };
}