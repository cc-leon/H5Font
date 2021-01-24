#include <stdarg.h>
#include <stdio.h>
#include <strsafe.h>

#include "constants.h"
#include "utils.h"

namespace mem {
    LPVOID GetMem(SIZE_T cbSize) {
        return VirtualAlloc(NULL,  // LPVOID lpAddress,
            cbSize,  // SIZE_T dwSize,
            MEM_COMMIT,  // DWORD flAllocationType,
            PAGE_READWRITE);  // DWORD  flProtect
    }

    BOOL FreeMem(LPVOID lpBuf) {
        return VirtualFree(lpBuf,  // LPVOID lpAddress,
            0,  // SIZE_T dwSize,
            MEM_RELEASE);  // DWORD  dwFreeType
    }

    SIZE_T AppendBuffer(LPBYTE abBuffer, SIZE_T cbBuffer, SIZE_T iCursor, LPCVOID lpObj, size_t cbObj) {
        memcpy_s(abBuffer + iCursor, cbBuffer - iCursor, lpObj, cbObj);
        return iCursor + cbObj;
    }
}

namespace file {
    BOOL FileExists(LPCTSTR szFileName) {
        WIN32_FIND_DATA FindFileData;
        HANDLE hFile = FindFirstFile(szFileName, &FindFileData);
        BOOL bResult = (hFile != INVALID_HANDLE_VALUE);

        if (bResult) {
            FindClose(hFile);
        }

        return bResult;
    }

    HANDLE GetFileReadHandle(LPCTSTR szFileName) {
        return CreateFile(szFileName,  // LPCSTR lpFileName,
                          GENERIC_READ,  // DWORD dwDesiredAccess,
                          0,  //  DWORD dwShareMode,
                          NULL,  // LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                          OPEN_EXISTING,  // DWORD dwCreationDisposition,
                          FILE_ATTRIBUTE_NORMAL,  // DWORD dwFlagsAndAttributes,
                          NULL);  // HANDLE TemplateFile
    }

    HANDLE GetFileWriteHandle(LPCTSTR szFileName) {
        if (file::FileExists(szFileName)) {
            DeleteFile(szFileName);
        }

        return CreateFile(szFileName,  // LPCSTR lpFileName,
                          GENERIC_WRITE,  // DWORD dwDesiredAccess,
                          0,  //  DWORD dwShareMode,
                          NULL,  // LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                          OPEN_ALWAYS,  // DWORD dwCreationDisposition,
                          FILE_ATTRIBUTE_NORMAL,  // DWORD dwFlagsAndAttributes,
                          NULL);  // HANDLE TemplateFile
    }

    BOOL CloseFileHandle(HANDLE hFile) {
        if (hFile != INVALID_HANDLE_VALUE && hFile != NULL) {
            return CloseHandle(hFile);
        }
        else {
            return FALSE;
        }
    }
}

namespace str {
    INT Unicode2LPTSTR(WCHAR iUnicode, LPTSTR lpszTString, SIZE_T cszMax) {
#ifdef _UNICODE
        lpszTString[0] = iUnicode;
        lpszTString[1] = 0;
        return 1;
#else
        WCHAR szUnicode[2];
        szUnicode[0] = iUnicode;
        szUnicode[1] = 0;
        return WideCharToMultiByte(CODE_PAGE,  // UINT CodePage
                                   0,  // DWORD dwFlags,
                                   szUnicode,  // LPCWCH lpWideCharStr,
                                   -1, // int cchWideChar,
                                   lpszTString, // LPSTR lpMultiByteStr,
                                   cszMax, // int cbMultiByte,
                                   NULL,  // LPCCH lpDefaultChar,
                                   NULL); // LPBOOL lpUsedDefaultChar
#endif
    }

    size_t GetTStrLen(LPCTSTR szSource, size_t cchMax) {
        size_t cchResult;
        if (StringCchLength(szSource, cchMax, &cchResult) == S_OK) {
            return cchResult;
        }
        else {
            return 0;
        }
    }

    LPTSTR CloneTStr(LPCTSTR szSource, size_t cchMax) {
        size_t cchResult = GetTStrLen(szSource, cchMax) + 1;
        if (cchResult == 0) {
            return NULL;
        }

        LPTSTR szResult = (LPTSTR)mem::GetMem(sizeof(TCHAR) * (cchResult));
        if (StringCchCopy(szResult, cchResult, szSource) == S_OK) {
            return szResult;
        }
        else {
            return NULL;
        }
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
