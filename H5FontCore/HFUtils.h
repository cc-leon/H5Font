#pragma once

#define HFSTRC(nID) str::LoadRcString(nID)

typedef struct tagUNICODEINFO{
    enum {
        L_BOUND,
        T_BOUND,
        R_BOUND,
        B_BOUND,
        L_OFFSET,
        R_OFFSET,
        UNKNOWN,
        POS_COUNT,
    };
    WCHAR wcUnicode;
    INT32 aiPos[POS_COUNT];
} UNICODEINFO, * PUNICODEINFO, * LPUNICODEINFO;

typedef UNICODEINFO CONST * LPCUNICODEINFO;

typedef struct tagFONTINFO {
    TCHAR szFacenam[LF_FACESIZE];
    INT nHeight;
    INT nPadding;
    INT nVPosition;
    INT nWeight;
    BYTE bItalic;
    BYTE bUnderline;
} FONTINFO, * PFONTINFO, * LPFONTINFO;
typedef FONTINFO CONST* LPCFONTINFO;


namespace sys {
    class __sys {
    public:
        __sys();
        ~__sys();
        UINT CodePage() CONST;
        CStringW LocaleName() CONST;
        size_t FillUnicodes(LPWSTR awcUnicodes) CONST;

    private:
        VOID __fillZhCNUnicodes();
        VOID __fillEnGBUnicodes();

        UINT m_uiCodePage;
        CStringW m_wsLocaleName;

        LPWSTR m_awcUnicodes;
        size_t m_cwcUnicodes;
    } const info;

    CString RunExe(LPCTSTR lpCmd, LPDWORD lpdwExitCode=NULL);
}

//
// Memory management functions
//
namespace mem {

    // Get new memory
    template<typename T>T* GetMem(SIZE_T cbSize) {
        int a = sizeof(T);
        return reinterpret_cast<T*>(::HeapAlloc(
            ::GetProcessHeap(),  // HANDLE hHeap,
            NULL,  // DWORD  dwFlags,
            cbSize * sizeof(T)));  // SIZE_T dwBytes
    }

    // Free the memory granted by GetMem
    template<typename T>BOOL FreeMem(T* & lpBuf) {
        if (lpBuf != NULL) {
            BOOL bResult = ::HeapFree(
                ::GetProcessHeap(),  // HANDLE hHeap,
                NULL, //DWORD dwFlags,
                lpBuf);  //_Frees_ptr_opt_ LPVOID lpMem
            if (bResult) {
                lpBuf = NULL;
            }
            return bResult;
        }
        else {
            return FALSE;
        }
    }

    // Copy additional content into a buffer
    SIZE_T AppendBuffer(LPBYTE abBuffer, SIZE_T cbBuffer, SIZE_T iCursor, LPCVOID lpObj, size_t cbObj);
}

//
// Unicode, text and buffer related functions
//
namespace str {
    // Convert a unicode short integer to a string containing this character
    INT Unicode2LPTSTR(WCHAR iUnicode, LPTSTR szTString, size_t cszMax);

    CStringA CStringW2CStringA(LPCWSTR wszText);

    CStringW CStringA2CStringW(LPCSTR szText);

    CString CStringW2CString(LPCWSTR wszText);

    CString CStringA2CString(LPCSTR szText);

    CStringA CString2CStringA(LPCTSTR szText);

    CStringW CString2CStringW(LPCTSTR szText);

    // Load a string resouce
    CString LoadRcString(UINT uiResourceID);

    // Convert the byte to a readable string
    CString Bytes2String(LPBYTE lpByte, SIZE_T cbByte);
}

//
// File functions
//
namespace file {

    // Check if a file exists on disk
    // Return TRUE only if there is such a name and the name points to a file
    BOOL FileExists(LPCTSTR szFileName);

    // Check if a folder exists on disk
    // Return TRUE only if there is such a name and the name points to a file
    BOOL FolderExists(LPCTSTR szPath);

    // Creates a folder if not exists recursively
    VOID CreateFolderIfNotExists(LPCTSTR szPath);

    // Expand a path to full ppath by eliminating the '.' or '..' parts
    CString GetAbsPath(LPCTSTR szPath);

    // Clear the content in a folder
    // bDeleteSelf will remove the folder itself after done
    DWORD ClearFolder(LPCTSTR szPath, BOOL bDeleteSelf=FALSE);

}

//
// Other functions
//

// Use WinAPI to print unicode characters in console
VOID PrintTf(TCHAR CONST* fmt, ...);
