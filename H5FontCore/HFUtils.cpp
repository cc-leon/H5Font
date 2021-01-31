#include "pch.h"

#include <stdarg.h>
#include <stdio.h>

#include "HFUtils.h"

__sys::__sys() {
    WCHAR szLocaleName[LOCALE_NAME_MAX_LENGTH];
    ::GetSystemDefaultLocaleName(szLocaleName, LOCALE_NAME_MAX_LENGTH);
    m_wsLocaleName = szLocaleName;

    if (m_wsLocaleName == L"zh-CN") {
        m_uiCodePage = 936;  // Simplified Chinese Code page
    }
    else if (m_wsLocaleName == L"zh-TW") {
        m_uiCodePage = 950;  // Traditional Chinese Code page
    }
    else {              // L"en-GB" English
        m_uiCodePage = CP_UTF8;
    }
}

UINT __sys::CodePage() CONST {
    return m_uiCodePage;
}

CStringW __sys::LocaleName() CONST {
    return m_wsLocaleName;
}

namespace mem {
    LPVOID GetMem(SIZE_T cbSize) {
        return ::HeapAlloc(
            ::GetProcessHeap(),  // HANDLE hHeap,
            NULL,  // DWORD  dwFlags,
            cbSize);  // SIZE_T dwBytes
    }

    BOOL FreeMem(LPVOID lpBuf) {
        return ::HeapFree(
            ::GetProcessHeap(),  // HANDLE hHeap,
            NULL, //DWORD dwFlags,
            lpBuf);  //_Frees_ptr_opt_ LPVOID lpMem
    }

    SIZE_T AppendBuffer(LPBYTE abBuffer, SIZE_T cbBuffer, SIZE_T iCursor, LPCVOID lpObj, size_t cbObj) {
        ::memcpy_s(abBuffer + iCursor, cbBuffer - iCursor, lpObj, cbObj);
        return iCursor + cbObj;
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

        LPTSTR szResult = (LPTSTR)mem::GetMem(sizeof(TCHAR) * cchResult);
        cchResult = ::GetFullPathName(
            szPath,   // LPCSTR lpFileName,
            cchResult,   // DWORD nBufferLength,
            szResult, //LPSTR lpBuffer,
            NULL);//LPSTR * lpFilePart

        CString sResult(szResult);
        mem::FreeMem((LPVOID)szResult);
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
            sys.CodePage(),  // UINT CodePage
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
            sys.CodePage(),  // UINT CodePage
            NULL,            // DWORD dwFlags,
            wszText,         // LPCWCH lpWideCharStr,
            -1,              // int cchWideChar,
            NULL,            // LPSTR lpMultiByteStr,
            NULL,            // int cbMultiByte,
            NULL,            // LPCCH lpDefaultChar,
            NULL);           // LPBOOL lpUsedDefaultChar
        LPSTR szResult = (LPSTR)mem::GetMem(sizeof(CHAR) * cchConverted);

        cchConverted = ::WideCharToMultiByte(
            sys.CodePage(),  // UINT CodePage
            NULL,            // DWORD dwFlags,
            wszText,         // LPCWCH lpWideCharStr,
            -1,              // int cchWideChar,
            szResult,            // LPSTR lpMultiByteStr,
            sizeof(TCHAR) * cchConverted, // int cbMultiByte,
            NULL,            // LPCCH lpDefaultChar,
            NULL);           // LPBOOL lpUsedDefaultChar

        CStringA sResult(szResult);
        mem::FreeMem((LPVOID)szResult);
        return sResult;
    }

    CStringW CStringA2CStringW(LPCSTR szText) {
        int cchConverted = ::MultiByteToWideChar(
            sys.CodePage(),  // UINT CodePage
            NULL,            // DWORD dwFlags,
            szText,          // LPCCH lpMultiByteStr,
            -1,              // int cbMultiByte,
            NULL,            // LPWSTR lpWideCharStr,
            NULL);           // int cchWideChar

        LPWSTR swzResult = (LPWSTR)mem::GetMem(sizeof(WCHAR) * cchConverted);
        cchConverted = ::MultiByteToWideChar(
            sys.CodePage(),  // UINT CodePage
            NULL,            // DWORD dwFlags,
            szText,          // LPCCH lpMultiByteStr,
            -1,              // int cbMultiByte,
            swzResult,       // LPWSTR lpWideCharStr,
            sizeof(WCHAR) * cchConverted);  // int cchWideChar

        CStringW sResult(swzResult);
        mem::FreeMem((LPVOID)szResult);
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
        int cchConverted = ::MultiByteToWideChar(
            sys.CodePage(),  // UINT CodePage
            NULL,            // DWORD dwFlags,
            szText,          // LPCCH lpMultiByteStr,
            -1,              // int cbMultiByte,
            NULL,            // LPWSTR lpWideCharStr,
            NULL);           // int cchWideChar

        LPTSTR szResult = (LPTSTR)mem::GetMem(sizeof(TCHAR) * cchConverted);
        cchConverted = ::MultiByteToWideChar(
            sys.CodePage(),  // UINT CodePage
            NULL,            // DWORD dwFlags,
            szText,          // LPCCH lpMultiByteStr,
            -1,              // int cbMultiByte,
            NULL,            // LPWSTR lpWideCharStr,
            NULL);           // int cchWideChar

        CString sResult(szResult);
        mem::FreeMem((LPVOID)szResult);
        return sResult;

#else
        return szText;
#endif
    }

    CStringA CString2CStringA(LPCTSTR szText) {
    }

    CStringW CString2CStringW(LPCSTR szText) {
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
