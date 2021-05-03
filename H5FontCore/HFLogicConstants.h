#pragma once

namespace HFLC {
    namespace sys {
        TCHAR CONST ADD_CHARS[] = _T("extended_chars.txt");
    }

    namespace header {
        CSize CONST DEF_DIM(4096, 4096*3);

        enum {
            BUTTON_16,
            DEFAULT_18, DEFAULT_20, DEFAULT_22, DEFAULT_26, DEFAULT_32,
            HEADER_22, HEADER_26, HEADER_32,
            SYSTEM_10, SYSTEM_16, SYSTEM_24,
            HEADER_COUNT
        };

        TCHAR CONST CODE_TO_LPTSTR[HEADER_COUNT][0x0f] = {
            _T("Button_16"),
            _T("Default_18"), _T("Default_20"), _T("Default_22"), _T("Default_26"), _T("Default_32"), 
            _T("Header_22"), _T("Header_26"), _T("Header_32"),
            _T("System_10"), _T("System_16"), _T("System_24") };

        INT CONST DEFAULT_HEIGHT[] = {
            16,
            18, 20, 22, 26, 32,
            22, 26, 32,
            10, 16, 24};

        INT CONST DEFAULT_WEIGHT[] = {
            FW_NORMAL,
            FW_NORMAL, FW_NORMAL, FW_NORMAL, FW_NORMAL, FW_NORMAL,
            FW_SEMIBOLD, FW_SEMIBOLD, FW_SEMIBOLD,
            FW_NORMAL, FW_NORMAL, FW_NORMAL };

        int LPSTR_TO_CODE(LPCTSTR szHeader);

    }

}