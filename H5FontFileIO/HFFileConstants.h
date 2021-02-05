#pragma once

namespace HFFC {
    namespace pak {
        TCHAR CONST XML_PATH[] = _T("UI\\Fonts");
        TCHAR CONST XML_BININFO[] = _T("(Font).xdb");
        TCHAR CONST BIN_PATH[] = _T("Bin\\Fonts");
        TCHAR CONST TEMP_FOLDER[] = _T("outputs\\temp");
    }

    //
    // The following are code flags from the bin file
    //
    namespace bin {
        // In the 1st part of the bin file, all unicode has this WOD
        WORD CONST UNICODE_FLAG = 0x0401;

        // In the 2nd part of the bin file, each position values are separated by this WORD
        WORD CONST POSITION_FLAG = 0x3802;
    }

    namespace xml {
        CHAR CONST XML_FONT_NODE[] = "Font";
        CHAR CONST XML_UID_SUBNODE[] = "uid";
    }

    namespace exe {
        TCHAR CONST ZIP_CMD[] = _T("libs\\7z.exe");
    }
}