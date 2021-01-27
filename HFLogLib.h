#pragma once

namespace logger {
    enum {
        MESSAGE_ID = WM_APP + 0x1000,
    };

    struct LOGMSG {
        LPTSTR szMsg = NULL;
        size_t cchMsg = 0;
        COLORREF crText = 0;
        BOOL bBold = FALSE;
        BOOL bItalic = FALSE;
        BOOL bUnderline = FALSE;
        BOOL bNewLine = FALSE;
        VOID SetMsg(LPCTSTR szSource);
        LOGMSG() = default;
        LOGMSG(LOGMSG CONST& other) = delete;
        LOGMSG(LOGMSG && other) = delete;
        VOID operator = (LOGMSG CONST& other) = delete;
        VOID operator = (LOGMSG&& other) = delete;

        ~LOGMSG();
    };
}
