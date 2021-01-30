#pragma once

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

}

namespace ui {
}

//
// File functions
//
namespace file {

    // Check if a file exists on disk
    BOOL FileExists(LPCTSTR szFileName);

    // Return a file handle to read an existing file
    HANDLE GetFileReadHandle(LPCTSTR szFileName);

    // Return a file handle to write to a file. Delete existing content if applicable
    HANDLE GetFileWriteHandle(LPCTSTR szFileName);

    // Close the file handle without giving exception during debug
    BOOL CloseFileHandle(HANDLE hFile);
}


//
// Other functions
//

// Use WinAPI to print unicode characters in console
VOID PrintTf(TCHAR CONST* fmt, ...);
