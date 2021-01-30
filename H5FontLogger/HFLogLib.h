#pragma once

namespace logger {
    enum {
        // For show a rich formated text, send from Logger singleton
        // LPARAM is a LOGMSG structure pointer that contains all formatting information,
        // WPARAM is NULL. Sender handles cleanup
        LOG_SHOW_TEXT = WM_APP + 0x0001,

        // To save the log into a file, send from Logger singleton
        // LPARAM is a LPTSTR pointer that contains filename,
        // WPARAM is NULL. Sender handles cleanup
        LOG_SAVE_TEXT,
    };

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
