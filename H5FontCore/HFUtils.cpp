#include "pch.h"
#include "HFUtils.h"

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
    BOOL FileExists(LPCTSTR szFileName) {
        WIN32_FIND_DATA FindFileData;
        HANDLE hFile = ::FindFirstFile(szFileName, &FindFileData);
        BOOL bResult = (hFile != INVALID_HANDLE_VALUE);

        if (bResult) {
            ::FindClose(hFile);
        }

        return bResult;
    }

    HANDLE GetFileReadHandle(LPCTSTR szFileName) {
        return ::CreateFile(
            szFileName,  // LPCSTR lpFileName,
            GENERIC_READ,  // DWORD dwDesiredAccess,
            0,  //  DWORD dwShareMode,
            NULL,  // LPSECURITY_ATTRIBUTES lpSecurityAttributes,
            OPEN_EXISTING,  // DWORD dwCreationDisposition,
            FILE_ATTRIBUTE_NORMAL,  // DWORD dwFlagsAndAttributes,
            NULL);  // HANDLE TemplateFile
    }

    HANDLE GetFileWriteHandle(LPCTSTR szFileName) {
        if (file::FileExists(szFileName)) {
            ::DeleteFile(szFileName);
        }

        return ::CreateFile(
            szFileName,  // LPCSTR lpFileName,
            GENERIC_WRITE,  // DWORD dwDesiredAccess,
            0,  //  DWORD dwShareMode,
            NULL,  // LPSECURITY_ATTRIBUTES lpSecurityAttributes,
            OPEN_ALWAYS,  // DWORD dwCreationDisposition,
            FILE_ATTRIBUTE_NORMAL,  // DWORD dwFlagsAndAttributes,
            NULL);  // HANDLE TemplateFile
    }

    BOOL CloseFileHandle(HANDLE hFile) {
        if (hFile != INVALID_HANDLE_VALUE && hFile != NULL) {
            return ::CloseHandle(hFile);
        }
        else {
            return FALSE;
        }
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
            LogicConst::CODE_PAGE,  // UINT CodePage
            0,  // DWORD dwFlags,
            szUnicode,  // LPCWCH lpWideCharStr,
            -1, // int cchWideChar,
            lpszTString, // LPSTR lpMultiByteStr,
            cszMax, // int cbMultiByte,
            NULL,  // LPCCH lpDefaultChar,
            NULL); // LPBOOL lpUsedDefaultChar
#endif
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
