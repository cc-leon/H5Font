#pragma once

namespace HFFC {
    namespace parser {
        TCHAR CONST TARGET_PAK[] = _T("data\\texts.pak");
        TCHAR CONST FALLBACK_PAK[] = _T("data\\data.pak");
        CHAR CONST XML_PATH[] = "UI/Fonts";
        CHAR CONST XML_BININFO[] = "(Font).xdb";
        CHAR CONST XML_FONT_NODE[] = "Font";
        CHAR CONST XML_UID_SUBNODE[] = "uid";
        CHAR CONST BIN_PATH[] = "Bin/Fonts";
    }

    //
    // The following are code flags from the bin file
    //
    namespace bin {
        // In the 1st part of the bin file, all unicode has this WOD
        INT16 CONST UNICODE_FLAG = 0x0401;

        // In the 2nd part of the bin file, each position values are separated by this WORD
        INT16 CONST POSITION_FLAG = 0x0328;
    }

    namespace output {
        TCHAR CONST BIN_FOLDER[] = _T("outputs\\temp\\Bin\\Fonts");
        TCHAR CONST UI_FOLDER[] = _T("outputs\\temp\\UI\\Fonts");
        TCHAR CONST TEMP_FOLDER[] = _T("outputs\\temp");
    }
}