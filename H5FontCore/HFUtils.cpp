#include "pch.h"

#include <stdarg.h>
#include <stdio.h>

#include "HFUtils.h"
#include "HFBitMask.h"

namespace sys {
    __sys::__sys(): m_awcUnicodes(NULL), m_cwcUnicodes(0), m_pbmMask(NULL) {
        WCHAR szLocaleName[LOCALE_NAME_MAX_LENGTH];
        ::GetSystemDefaultLocaleName(szLocaleName, LOCALE_NAME_MAX_LENGTH);
        m_wsLocaleName = szLocaleName;

        if (m_wsLocaleName == L"zh-CN") {
            m_uiCodePage = 936;  // Simplified Chinese Code page
            __fillZhCNUnicodes();
        }
        else if (m_wsLocaleName == L"zh-TW") {
            m_uiCodePage = 950;  // Traditional Chinese Code page
        }
        else {                  // L"en-GB" English
            m_uiCodePage = CP_UTF8;
            __fillEnGBUnicodes();
        }
    }

    __sys::~__sys() {
        mem::FreeMem(m_awcUnicodes);
        if (m_pbmMask) {
            delete m_pbmMask;
        }
    }
    __sys const info;

    UINT __sys::CodePage() CONST {
        return m_uiCodePage;
    }

    CStringW __sys::LocaleName() CONST {
        return m_wsLocaleName;
    }

    size_t __sys::FillUnicodes(LPWSTR awcUnicodes) CONST {
        if (awcUnicodes != NULL) {
            for (size_t i = 0; i < m_cwcUnicodes; i++) {
                awcUnicodes[i] = m_awcUnicodes[i];
            }
        }
        return m_cwcUnicodes;
    }

    HFBitMask CONST* __sys::GetPaddingCharSet() CONST {
        return m_pbmMask;
    }

    VOID __sys::__fillZhCNUnicodes() {
        // File GB2312 codes
        if (m_pbmMask) {
            delete m_pbmMask;
        }
        m_pbmMask = new HFBitMask(0xFFFF);

        BYTE CONST ANSI_RANGE[] = {0x20, 0x7e};
        BYTE CONST HIBYTES_RANGES[][2] = {{ 1, 9 }, { 16, 55 }, { 56, 87 }, { 88, 89 }};
        size_t CONST HIBYTE_1ST_DIM = 4;
        size_t CONST HIBYTE_PADDING = 16;
        BYTE CONST LOBYTES_RANGE[] = {01, 94};
        BYTE CONST GB_OFFSET = 0xA0;

        m_cwcUnicodes = ANSI_RANGE[1] - ANSI_RANGE[0] + 1;
        for (size_t i = 0; i < HIBYTE_1ST_DIM; i++) {
            for (WCHAR j = HIBYTES_RANGES[i][0]; j <= HIBYTES_RANGES[i][1]; j++) {
                for (WCHAR k = LOBYTES_RANGE[0]; k <= LOBYTES_RANGE[1]; k++) {
                    m_cwcUnicodes++;
                }
            }
        }

        mem::FreeMem(m_awcUnicodes);
        m_awcUnicodes = mem::GetMem<WCHAR>(m_cwcUnicodes);
        size_t i = 0;
        for (WCHAR j = ANSI_RANGE[0]; j <= ANSI_RANGE[1]; j++) {
            m_awcUnicodes[i++] = j;
        }
        for (size_t j = 0; j < HIBYTE_1ST_DIM; j++) {
            for (WCHAR k = HIBYTES_RANGES[j][0]; k <= HIBYTES_RANGES[j][1]; k++) {
                for (WCHAR l = LOBYTES_RANGE[0]; l <= LOBYTES_RANGE[1]; l++) {
                    CHAR gb2312Char[3] = { 0 };
                    gb2312Char[0] = k + GB_OFFSET;
                    gb2312Char[1] = l + GB_OFFSET;
                    WCHAR wcUnicode[2];
                    ::MultiByteToWideChar(m_uiCodePage, NULL, gb2312Char,-1, wcUnicode, 2);
                    m_awcUnicodes[i++] = wcUnicode[0];
                    if (k >= HIBYTE_PADDING) {
                        (*m_pbmMask)[size_t(wcUnicode[0])] = TRUE;
                    }
                }
            }
        }

    }

    VOID __sys::__fillEnGBUnicodes() {
    }



    CString RunExe(LPCTSTR lpCmd, LPDWORD lpdwExitCode, LPCTSTR szWorkingDir) {

        SECURITY_ATTRIBUTES sa;
        ::ZeroMemory(&sa, sizeof(sa));
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;

        CString sTemp;
        TCHAR szTempFolder[MAX_PATH];
        TCHAR szTempFullName[MAX_PATH];

        if (!::GetTempPath(MAX_PATH, szTempFolder)) {
            LOGAPI("GetTempPath", 0, _T(""));
        }
        if (!::GetTempFileName(
            szTempFolder,        // LPCTSTR lpPathName
            _T("H5FontRunExe"),  // LPCTSTR lpPrefixString
            0,                   // UINT uUnique
            szTempFullName)) {     // LPTSTR lpTempFileName
            LOGAPI("GetTempFileName", 0, _T(""));
        }

        HANDLE hTempFile = ::CreateFile(
            (LPTSTR)szTempFullName,             // LPCTSTR lpFileName
            GENERIC_WRITE | GENERIC_READ,       // DWORD dwDesiredAccess
            FILE_SHARE_READ | FILE_SHARE_WRITE,  // DWORD dwShareMode
            &sa,                                // LPSECURITY_ATTRIBUTES lpSecurityAttributes
            CREATE_ALWAYS,                      //DWORD dwCreationDisposition
            FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, // DWORD dwFlagsAndAttributes
            NULL);                              //HANDLE hTemplateFile*/
        if (hTempFile == INVALID_HANDLE_VALUE) {
            LOGAPI("CreateFile", hTempFile, szTempFullName);
        }

        STARTUPINFO si;
        ::ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdInput = NULL;
        si.hStdError = hTempFile;
        si.hStdOutput = hTempFile;

        PROCESS_INFORMATION pi;
        ::ZeroMemory(&pi, sizeof(pi));

        BOOL bResult =::CreateProcess(
            NULL,                        // LPCSTR lpApplicationName,
            CString(lpCmd).GetBuffer(),  // LPSTR lpCommandLine,
            NULL,                        // LPSECURITY_ATTRIBUTES lpProcessAttributes,
            NULL,                        // LPSECURITY_ATTRIBUTES lpThreadAttributes,
            TRUE,                        // BOOL bInheritHandles,
            CREATE_NO_WINDOW,            // DWORD dwCreationFlags,
            NULL,                        // LPVOID lpEnvironment,
            szWorkingDir,                        // LPCSTR lpCurrentDirectory,
            &si,                         // LPSTARTUPINFOA lpStartupInfo,
            &pi);                        // LPPROCESS_INFORMATION lpProcessInformation
        if (!bResult) {
            LOGAPI("CreateProcess", bResult, lpCmd);
        }
        ::WaitForSingleObject(pi.hProcess, INFINITE);
        if (lpdwExitCode != NULL) {
            ::GetExitCodeProcess(pi.hProcess, lpdwExitCode);
        }
        ::CloseHandle(pi.hThread);
        ::CloseHandle(pi.hProcess);

        ::SetFilePointer(hTempFile, 0, NULL, FILE_BEGIN);
        DWORD dwTempFileSize = ::GetFileSize(hTempFile, NULL);
        DWORD dwTempFileBytesRead;
        LPBYTE lpBufStdOut = mem::GetMem<BYTE>(dwTempFileSize + 1);
        bResult =::ReadFile(
            hTempFile,
            lpBufStdOut,
            dwTempFileSize,
            &dwTempFileBytesRead,
            NULL);
        if (!bResult) {
            LOGAPI("ReadFile", bResult, lpCmd);
        }
        lpBufStdOut[dwTempFileSize] = _T('\0');

        CString sResult;
        if (::IsTextUnicode(lpBufStdOut, dwTempFileSize, NULL)) {
            sResult = str::CStringW2CString((LPCWSTR)lpBufStdOut);
        }
        else {
            sResult = str::CStringA2CString((LPCSTR)lpBufStdOut);
        }
        mem::FreeMem(lpBufStdOut);
        ::CloseHandle(hTempFile);
        return sResult;
    }
}

namespace mem {

    SIZE_T AppendBuffer(LPBYTE abBuffer, SIZE_T cbBuffer, SIZE_T iCursor, LPCVOID lpObj, size_t cbObj) {
        ::memcpy_s(abBuffer + iCursor, cbBuffer - iCursor, lpObj, cbObj);
        return iCursor + cbObj;
    }
}

namespace str {
    INT Unicode2LPTSTR(WCHAR iUnicode, LPTSTR lpszTString, size_t cszMax) {
#ifdef _UNICODE
        lpszTString[0] = iUnicode;
        lpszTString[1] = 0;
        return 1;
#else
        WCHAR szUnicode[2];
        szUnicode[0] = iUnicode;
        szUnicode[1] = 0;
        return ::WideCharToMultiByte(
            sys::info.CodePage(),  // UINT CodePage
            0,  // DWORD dwFlags,
            szUnicode,  // LPCWCH lpWideCharStr,
            -1, // int cchWideChar,
            lpszTString, // LPSTR lpMultiByteStr,
            cszMax, // int cbMultiByte,
            NULL,  // LPCCH lpDefaultChar,
            NULL); // LPBOOL lpUsedDefaultChar
#endif
    }


    CStringA CStringW2CStringA(LPCWSTR wszText) {
        int cchConverted = ::WideCharToMultiByte(
            sys::info.CodePage(),  // UINT CodePage
            NULL,            // DWORD dwFlags,
            wszText,         // LPCWCH lpWideCharStr,
            -1,              // int cchWideChar,
            NULL,            // LPSTR lpMultiByteStr,
            NULL,            // int cbMultiByte,
            NULL,            // LPCCH lpDefaultChar,
            NULL);           // LPBOOL lpUsedDefaultChar
        LPSTR szResult = mem::GetMem<CHAR>(cchConverted);

        cchConverted = ::WideCharToMultiByte(
            sys::info.CodePage(),  // UINT CodePage
            NULL,            // DWORD dwFlags,
            wszText,         // LPCWCH lpWideCharStr,
            -1,              // int cchWideChar,
            szResult,            // LPSTR lpMultiByteStr,
            cchConverted, // int cbMultiByte,
            NULL,            // LPCCH lpDefaultChar,
            NULL);           // LPBOOL lpUsedDefaultChar

        CStringA sResult(szResult);
        mem::FreeMem(szResult);
        return sResult;
    }

    CStringW CStringA2CStringW(LPCSTR szText) {
        int cchConverted = ::MultiByteToWideChar(
            sys::info.CodePage(),  // UINT CodePage
            NULL,            // DWORD dwFlags,
            szText,          // LPCCH lpMultiByteStr,
            -1,              // int cbMultiByte,
            NULL,            // LPWSTR lpWideCharStr,
            NULL);           // int cchWideChar

        LPWSTR swzResult = mem::GetMem<WCHAR>(cchConverted);
        cchConverted = ::MultiByteToWideChar(
            sys::info.CodePage(),  // UINT CodePage
            NULL,            // DWORD dwFlags,
            szText,          // LPCCH lpMultiByteStr,
            -1,              // int cbMultiByte,
            swzResult,       // LPWSTR lpWideCharStr,
            cchConverted);  // int cchWideChar

        CStringW sResult(swzResult);
        mem::FreeMem(swzResult);
        return sResult;
    }

    CString CStringW2CString(LPCWSTR wszText) {
#ifdef _UNICODE
        return wszText;
#else
        return CStringW2CStringA(wszText);
#endif
    }

    CString CStringA2CString(LPCSTR szText) {
#ifdef _UNICODE
        return CStringA2CStringW(szText);
#else
        return szText;
#endif
    }

    CStringA CString2CStringA(LPCTSTR szText) {
#ifdef _UNICODE
        return CStringW2CStringA(szText);
#else
        return szText;
#endif
    }

    CStringW CString2CStringW(LPCTSTR szText) {
#ifdef _UNICODE
        return szText;
#else
        return CStringA2CStringW(szText);;
#endif
    }

    CString LoadRcString(UINT uiResourceID) {
        CStringW wsResult;
        if (wsResult.LoadString(uiResourceID) == 0) {
            return CString();
        }

        return str::CStringW2CString(wsResult);
    }

    // Convert the byte to a readable string
    CString Bytes2String(LPBYTE lpByte, SIZE_T cbByte) {
        CString sResult = _T("     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
        for (SIZE_T i = 0; i < cbByte; i++) {
            CString sTemp;
            if (i % 0x10 == 0) {
                sTemp.Format(_T("\n%04x "), i / 0x10);
                sResult.Append(sTemp);
            }
            sTemp.Format(_T("%02x "), lpByte[i]);
            sResult.Append(sTemp);
        }
        return sResult;
    }

}

namespace file {
    BOOL FileExists(LPCTSTR szPath) {
        DWORD dwAttrib = ::GetFileAttributes(szPath);
        return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }

    BOOL FolderExists(LPCTSTR szPath) {
        DWORD dwAttrib = ::GetFileAttributes(szPath);
        return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

    VOID CreateFolderIfNotExists(LPCTSTR szPath) {
        if (!FolderExists(szPath)) {
            CString sPath = szPath;
            if (sPath[sPath.GetLength() - 1] == _T('\\')) {
                sPath.Delete(sPath.GetLength() - 1);
            }

            CreateFolderIfNotExists(GetAbsPath(sPath + CString(_T("\\.."))));
            ::CreateDirectory(sPath, NULL);
        }
    }

    CString GetAbsPath(LPCTSTR szPath) {
        DWORD cchResult = ::GetFullPathName(
            szPath,   // LPCSTR lpFileName,
            0,   // DWORD nBufferLength,
            NULL, //LPSTR lpBuffer,
            NULL);//LPSTR * lpFilePart

        LPTSTR szResult = mem::GetMem<TCHAR>(cchResult);
        cchResult = ::GetFullPathName(
            szPath,   // LPCSTR lpFileName,
            cchResult,   // DWORD nBufferLength,
            szResult, //LPSTR lpBuffer,
            NULL);//LPSTR * lpFilePart

        CString sResult(szResult);
        mem::FreeMem(szResult);
        return sResult;
    }

    DWORD ClearFolder(LPCTSTR szPath, BOOL bDeleteSelf) {
        DWORD dwResult = 0;
        WIN32_FIND_DATA wfdCurrFolder;
        CString sPath(szPath);
        if (sPath[sPath.GetLength() - 1] == _T('\\')) {
            sPath.Delete(sPath.GetLength() - 1);
        }

        HANDLE hFind = ::FindFirstFile(sPath + CString("\\*"), &wfdCurrFolder);

        if (hFind == INVALID_HANDLE_VALUE) {
            if (bDeleteSelf) {
                dwResult += ::RemoveDirectory(sPath);
            }
            return dwResult;
        }

        do {
            if (CString(wfdCurrFolder.cFileName) == _T(".") || CString(wfdCurrFolder.cFileName) == _T("..")) {
                continue;
            }

            CString sFilename = sPath + CString(_T("\\")) + wfdCurrFolder.cFileName;
            if (hFind != INVALID_HANDLE_VALUE) {
                if (wfdCurrFolder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    dwResult += ClearFolder(sFilename);
                    dwResult += ::RemoveDirectory(sFilename);
                }
                else {
                    dwResult += ::DeleteFile(sFilename);
                }
            }
        } while (::FindNextFile(hFind, &wfdCurrFolder));

        ::FindClose(hFind);

        if (bDeleteSelf) {
            dwResult += ::RemoveDirectory(sPath);
        }

        return dwResult;
    }

    SIZE_T DumpInFile(LPCTSTR szFilname, LPVOID lpData, SIZE_T cbData) {
        CFile cfDump(szFilname, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary);
        cfDump.Write(lpData, cbData);
        return cbData;
    }

    SIZE_T PickFromFile(LPCTSTR szFilename, LPVOID lpData) {
        CFile cfPick(szFilename, CFile::modeRead | CFile::typeBinary);
        if (lpData == NULL) {
            return (SIZE_T)cfPick.GetLength();
        }
        else {
            return cfPick.Read(lpData, (SIZE_T)cfPick.GetLength());
        }
    }

    BOOL ZipFile(LPCTSTR szZipname, LPCTSTR szDirname) {
        CString sLog;
        CString sCmd;
        if (file::FileExists(szZipname)) {
            ::DeleteFile(szZipname);
        }
        sLog.Format(HFSTRC(IDS_LOG_ARCHIVING_ZIP), szZipname);
        LOG.log(sLog, LOG.INFO);
        LOG.log(HFSTRC(IDS_LOG_NOW_EXECUTING), LOG.STD, TRUE);
        sCmd.Format(_T("%s a -tzip \"%s\" *"), HFFC::exe::ZIP_CMD, szZipname);
        LOG.log(sCmd, LOG.STD);
        DWORD dwExitCode = 0;
        LOG.log(sys::RunExe(sCmd, &dwExitCode, szDirname), LOG.STD);
        if (dwExitCode) {
            sLog.Format(HFSTRC(IDS_LOG_WRONG_EXIT_CODE), dwExitCode);
            return FALSE;
        }
        LOG.log(HFSTRC(IDS_LOG_FINISHED), LOG.NORM, TRUE);

        return TRUE;
    }

    BOOL RmoveFileFromZip(LPCTSTR szZipname, LPCTSTR szFilename) {
        return TRUE;
    }

    BOOL ClearTempFile() {
        LOG.log(HFSTRC(IDS_LOG_DELETING_TEMP), LOG.INFO);
        DWORD dwClearResult = file::ClearFolder(HFFC::pak::TEMP_FOLDER);
        CString sLog;
        sLog.Format(HFSTRC(IDS_LOG_DELETED_TEMP), dwClearResult);
        LOG.log(sLog, LOG.INFO);
        return TRUE;
    }
}

VOID PrintTf(TCHAR CONST* fmt, ...) {
    static DWORD const MAX_BUF_SIZE = 0x1000;
    TCHAR szBuf[MAX_BUF_SIZE];
    va_list va;
    va_start(va, fmt);
#ifdef _UNICODE
    vswprintf_s(szBuf, MAX_BUF_SIZE, fmt, va);
#else
    vsprintf_s(szBuf, MAX_BUF_SIZE, fmt, va);
#endif
    va_end(va);

    size_t cchBuf;
    (VOID)StringCchLength(szBuf, MAX_BUF_SIZE, &cchBuf);
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), (LPCVOID)szBuf, cchBuf, NULL, NULL);
}
