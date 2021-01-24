#pragma once
#include <windows.h>

#include "constants.h"

class Parser {
public:
    static DWORD CONST BLOCK_SIZE = 0x1000;
    Parser(LPCTSTR szBinFileName, SIZE_T cchMaxBinFileName,
           LPCTSTR szTxtFileName, SIZE_T cchMaxTxtFileName,
           BOOL bSilent=FALSE);
    ~Parser();
    Parser() = delete;
    Parser(Parser CONST&) = delete;
    Parser(Parser &&) = delete;
    VOID GenerateBinFile();

protected:
    VOID ReadAllBytes();
    VOID ScanBytes();
    VOID GenerateTxtFile();

    struct CHAR_INFO {
        WCHAR wcUnicode;
        INT32 iPos[POSITION_DW_COUNT];
    };

    LPTSTR m_szBinFile;
    LPTSTR m_szTxtFile;
    BOOL m_bSilent;
    LPBYTE m_lpBuf;
    SIZE_T m_cbBuf;
    CHAR_INFO* m_aciChars;
    SIZE_T m_cciChars;
};

