#pragma once

namespace logger {
    struct LOGMSG {
        CString sMsg;
        COLORREF crText = 0;
        BOOL bBold = FALSE;
        BOOL bItalic = FALSE;
        BOOL bUnderline = FALSE;
        BOOL bNewLine = FALSE;
        LOGMSG() = default;
        LOGMSG(LOGMSG CONST& other) = delete;
        LOGMSG(LOGMSG && other) = delete;
        VOID operator = (LOGMSG CONST& other) = delete;
        VOID operator = (LOGMSG&& other) = delete;
    };
}
