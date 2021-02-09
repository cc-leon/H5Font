#pragma once

namespace HFFC {
    //
    // The following are code flags from the bin file
    //
    namespace bin {
        // In the 1st part of the bin file, all unicode has this WOD
        WORD CONST UNICODE_FLAG = 0x0401;

        // In the 2nd part of the bin file, each position values are separated by this WORD
        WORD CONST POSITION_FLAG = 0x3802;

        SIZE_T CONST HEAD_COUNTS_ADDR[] = {0x1A, 0x1F, 0x2A, 0x5A, 0x60};

        SIZE_T CONST HEAD_COUNTS_FACTOR[] = {68, 68, 68, 68, 1};

        SIZE_T CONST HEAD_COUNTS_OFFSET[] = {181, 171, 149, 25, 0};

        size_t CONST HEAD_COUNTS_NUM = sizeof(HEAD_COUNTS_ADDR) / sizeof(SIZE_T);
    }

    namespace xml {
        CHAR CONST XML_FONT_NODE[] = "Font";
        CHAR CONST XML_UID_SUBNODE[] = "uid";
    }

    namespace pak {
        TCHAR CONST XML_PATH[] = _T("UI\\Fonts");
        TCHAR CONST XML_BININFO[] = _T("(Font).xdb");
        TCHAR CONST BIN_PATH[] = _T("Bin\\Fonts");
        TCHAR CONST TEMP_FOLDER[] = _T("outputs\\temp");
    }

    namespace exe {
        TCHAR CONST ZIP_CMD[] = _T("libs\\7z.exe");
        TCHAR CONST MAGICK_CMD[] = _T("libs\\magick.exe");
    }

}