#pragma once

namespace LogicConst {
    // Code page is GB2312 - Simplified Chinese, change it here if you are using another language
    // Check https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers for more code page codes.
    UINT CONST CODE_PAGE = 936;

    // Max number of characters supported by the binary file - Max Value of unsigned 16 bit integer
    size_t CONST MAX_CHARS = 0xFFFF;


    enum {
        BUTTON_16 = 0,
        DEFAULT_18, DEFAULT_20, DEFAULT_22, DEFAULT_26, DEFAULT_32, DEFAULT_40,
        HEADER_22, HEADER_26, HEADER_32, SYSTEM_10, SYSTEM_16, SYSTEM_24,
        INDEX_COUNT
    };

    //
    // The following are code flags from the bin file
    //

    // In the 1st part of the bin file, all unicode has this WOD
    INT16 CONST UNICODE_FLAG = 0x0401;
    // In the 2nd part of the bin file, each position values are separated by this WORD
    INT16 CONST POSITION_FLAG = 0x0328;

    //
    // The following are count constants from the bin file
    //

    // Each position values have this number of DWORD
    INT CONST POSITION_DW_COUNT = 7;

}