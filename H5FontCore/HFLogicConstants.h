#pragma once

namespace HFLC {

    // Max number of characters supported by the binary file - Max Value of unsigned 16 bit integer
    size_t CONST MAX_CHARS = 0xFFFF;

    //
    // The following are code flags for head bin file mapping
    //
    namespace header {
        enum {
            BUTTON_16 = 0,
            DEFAULT_18, DEFAULT_20, DEFAULT_22, DEFAULT_26, DEFAULT_32, DEFAULT_40,
            HEADER_22, HEADER_26, HEADER_32,
            SYSTEM_10, SYSTEM_16, SYSTEM_24,
            HEADER_COUNT
        };
        TCHAR CONST CODE_TO_LPTSTR[HEADER_COUNT][0x0f] = {
            _T("Button_16"),
            _T("Default_18"), _T("Default_20"), _T("Default_22"), _T("Default_26"), _T("Default_32"), _T("Default_40"),
            _T("Header_22"), _T("Header_26"), _T("Header_32"),
            _T("System_10"), _T("System_16"), _T("System_24") };
    }
}