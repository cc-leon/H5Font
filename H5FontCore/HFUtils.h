#pragma once

class __sys {
public:
    __sys();
    UINT CodePage() CONST;
    CStringW LocaleName() CONST;

private:
    UINT m_uiCodePage;
    CStringW m_wsLocaleName;

} const sys;

typedef struct tagUNICODEINFO{
    static INT CONST POSITION_DW_COUNT = 7;
    WCHAR wcUnicode;
    INT32 aiPos[POSITION_DW_COUNT];
} UNICODEINFO, * PUNICODEINFO, * LPUNICODEINFO;

typedef UNICODEINFO CONST * LPCUNICODEINFO;


typedef struct tagFONTINFO {
    INT iStyleName;
    TCHAR szFacenam[LF_FACESIZE];
    INT nHeight;
    INT nWeight;
    BYTE bItalic;
    BYTE bUnderline;
} FONTINFO, * PFONTINFO, * LPFONTINFO;
typedef FONTINFO CONST* LPCFONTINFO;

//
// Memory management functions
//
namespace mem {

    // Get new memory
    LPVOID GetMem(SIZE_T cbSize);

    // Free the memory granted by GetMem
    BOOL FreeMem(LPVOID lpBuf);

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
