#include <tchar.h>
#include <stdio.h>

#include "constants.h"
#include "utils.h"
#include "parser.h"

Parser::Parser(LPCTSTR szBinFileName, SIZE_T cchMaxBinFileName, 
               LPCTSTR szTxtFileName, SIZE_T cchMaxTxtFileName,
               BOOL bSilent) 
    : m_bSilent(bSilent), m_lpBuf(NULL), m_cbBuf(0), m_aciChars(NULL), m_cciChars(0) {

    m_szBinFile = str::CloneTStr(szBinFileName, cchMaxBinFileName);
    m_szTxtFile = str::CloneTStr(szTxtFileName, cchMaxTxtFileName);


    if (file::FileExists(szTxtFileName)) {
        DeleteFile(szTxtFileName);
    }

    ReadAllBytes();
    ScanBytes();
    GenerateTxtFile();
}

Parser::~Parser() {
    if (m_szBinFile != NULL) {
        mem::FreeMem(m_szBinFile);
    }

    if (m_szTxtFile != NULL) {
        mem::FreeMem(m_szTxtFile);
    }

    if (m_lpBuf != NULL) {
        mem::FreeMem(m_lpBuf);
    }

    if (m_aciChars != NULL) {
        mem::FreeMem(m_aciChars);
    }
}

VOID Parser::ReadAllBytes() {
    HANDLE hBinFile = file::GetFileReadHandle(m_szBinFile);
    if (hBinFile == NULL) {
        hBinFile = INVALID_HANDLE_VALUE;
    }

    if (m_bSilent == FALSE) {
        if (hBinFile == INVALID_HANDLE_VALUE) {
            PrintTf(_T("\"%s\" cannot be opened\n"), m_szBinFile);
        }
        else {
            PrintTf(_T("\"%s\" is opended successfully\n"), m_szBinFile);
        }
    }

    LARGE_INTEGER liLarge = {};
    GetFileSizeEx(hBinFile, &liLarge);
    m_cbBuf = SIZE_T(liLarge.QuadPart);
    m_lpBuf = (LPBYTE)mem::GetMem(m_cbBuf);

    DWORD  dwBytesRead;
    (VOID)ReadFile(hBinFile, m_lpBuf, m_cbBuf, &dwBytesRead, NULL);
    if (dwBytesRead != m_cbBuf) {
        PrintTf(_T("Bytes read and bytes from file size doesn't match.\n"));
    }
    else {
        if (m_bSilent == FALSE) {
            PrintTf(_T("%d bytes read from file\n"), dwBytesRead);
        }
    }

    file::CloseFileHandle(hBinFile);
}

VOID Parser::ScanBytes() {
    //Step 1. Scan for all unicodes

    m_cciChars = 0;
    WCHAR * CONST awcUnicodes = (WCHAR*)mem::GetMem(sizeof(WCHAR[MAX_CHARS]));
    LPDWORD lpdwReader = LPDWORD(m_lpBuf);

    for (UINT i = 0; i < m_cbBuf / sizeof(DWORD); i++) {
        lpdwReader++;
        if (HIWORD(*lpdwReader) == UNICODE_FLAG) {
            awcUnicodes[m_cciChars++] = LOWORD(*lpdwReader);
        }
    }

    if (m_bSilent == FALSE) {
        PrintTf(_T("%d characters read by 0x0401 keys\n"), m_cciChars);
    }

    m_aciChars = (CHAR_INFO*)mem::GetMem(sizeof(CHAR_INFO) * m_cciChars);
    for (SIZE_T i = 0; i < m_cciChars; i++) {
        m_aciChars[i].wcUnicode = awcUnicodes[i];
    }
    mem::FreeMem(awcUnicodes);

    //Step 2. Scan for all positions

    LPWORD lpwReader = LPWORD(m_lpBuf);
    SIZE_T cciChars = 0;
    for (UINT i = 0; i < m_cbBuf / sizeof(WORD); i++) {
        lpwReader++;
        if (*lpwReader == 0x3802) {
            INT32 * lpi32Reader = (INT32*)(lpwReader + 1);
            for (INT j = 0; j < POSITION_DW_COUNT; j++) {
                m_aciChars[cciChars].iPos[j] = *(lpi32Reader + j);
            }
            cciChars++;
        }
    }

    if (m_bSilent == FALSE) {
        PrintTf(_T("%d characters read from the flag 0x3802\n"), cciChars);
    }

}

VOID Parser::GenerateTxtFile() {
    SIZE_T cbBufferSize = 0xfffff;
    LPBYTE abBuffer = (LPBYTE)mem::GetMem(cbBufferSize);
    SIZE_T cbCursor = 0;

    TCHAR cSeparator = _T('\t');
    TCHAR cNewLine[] = _T("\r\n");
    size_t const cchMaxTextBuffer = 7;
    INT cchTextBuffer = 0;
    TCHAR szTextBuffer[cchMaxTextBuffer];

    for (SIZE_T i = 0; i < m_cciChars; i++) {
        cchTextBuffer = _stprintf_s(szTextBuffer, cchMaxTextBuffer, _T("%04x"), m_aciChars[i].wcUnicode);

        cbCursor = mem::AppendBuffer(abBuffer, cbBufferSize, cbCursor, 
                                     (LPVOID)szTextBuffer, cchTextBuffer * sizeof(TCHAR));
        cbCursor = mem::AppendBuffer(abBuffer, cbBufferSize, cbCursor,
                                     (LPVOID)&cSeparator, sizeof(TCHAR));

        TCHAR szChar[5];
        str::Unicode2LPTSTR(m_aciChars[i].wcUnicode, szChar, 5);
        cchTextBuffer = _stprintf_s(szTextBuffer, cchMaxTextBuffer, _T("\'%s\'"), szChar);
        cbCursor = mem::AppendBuffer(abBuffer, cbBufferSize, cbCursor,
                                     (LPVOID)szTextBuffer, cchTextBuffer * sizeof(TCHAR));
        cbCursor = mem::AppendBuffer(abBuffer, cbBufferSize, cbCursor,
                                     (LPVOID)&cSeparator, sizeof(TCHAR));

        for (INT j = 0; j < POSITION_DW_COUNT; j++) {
            cchTextBuffer = _stprintf_s(szTextBuffer, cchMaxTextBuffer, _T("%04d"), m_aciChars[i].iPos[j]);
            cbCursor = mem::AppendBuffer(abBuffer, cbBufferSize, cbCursor,
                                         (LPVOID)szTextBuffer, cchTextBuffer * sizeof(TCHAR));

            if (j != POSITION_DW_COUNT - 1) {
                cbCursor = mem::AppendBuffer(abBuffer, cbBufferSize, cbCursor,
                    (LPVOID)&cSeparator, sizeof(TCHAR));
            }

        }

        cbCursor = mem::AppendBuffer(abBuffer, cbBufferSize, cbCursor,
                                     (LPVOID)cNewLine, sizeof(TCHAR) * 2);
    }

    HANDLE hTxtFile = file::GetFileWriteHandle(m_szTxtFile);
    if (hTxtFile == NULL) {
        hTxtFile = INVALID_HANDLE_VALUE;
    }

    if (m_bSilent == FALSE) {
        if (hTxtFile == INVALID_HANDLE_VALUE) {
            PrintTf(_T("\"%s\" cannot be opened\n"), m_szTxtFile);
        }
        else {
            PrintTf(_T("\"%s\" is opended successfully\n"), m_szTxtFile);
        }
    }

    DWORD dwNumBytesWritten;
    WriteFile(hTxtFile, (LPVOID)abBuffer, cbCursor, &dwNumBytesWritten, NULL);

    mem::FreeMem(abBuffer);
    file::CloseFileHandle(hTxtFile);
}

