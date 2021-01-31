#pragma once

namespace LC {

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
            INDEX_COUNT
        };
        TCHAR CONST CODE_TO_LPTSTR[INDEX_COUNT][0x0f] = {
            _T("Button_16"),
            _T("Default_16"), _T("Default_20"), _T("Default_22"), _T("Default_26"), _T("Default_32"), _T("Default_40"),
            _T("HEADER_22"), _T("HEADER_26"), _T("HEADER_32"),
            _T("SYSTEM_10"), _T("SYSTEM_16"), _T("SYSTEM_24") };
    }

    //
    // The following are code flags from the bin file
    //
    namespace bin {
        // In the 1st part of the bin file, all unicode has this WOD
        INT16 CONST UNICODE_FLAG = 0x0401;

        // In the 2nd part of the bin file, each position values are separated by this WORD
        INT16 CONST POSITION_FLAG = 0x0328;

        // Each position values have this number of DWORD
        INT CONST POSITION_DW_COUNT = 7;
    }

}