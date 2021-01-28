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

    // Get the length of a string
    size_t GetTStrLen(LPCTSTR szSource, size_t cchMax);

    // Make a dynamic copy of the string, remember to free the memory afterwards
    LPTSTR CloneTStr(LPCTSTR szSource, size_t cchMax);

    // Concatenate two strings and make a new clone, remember to free the memory afterwards
    LPTSTR CatTStrs(LPCTSTR szSource1, size_t cchMax1, LPCSTR szSource2, size_t cchMax2);
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
// Functions used to help manipulation on GUI
//
namespace ui {
    // Get the CRect of a control in its parent window
    // hWnd is the handle to the control of interest
    CRect GetWindowCRectInParent(CWnd * pWnd);

    // Given a rectangle, its new size and the pivot point (inside the CRect), 
    // calculate the new CRect region it should position
    CRect ZoomCRectOverCPoint(CSize CONST& csNew, CRect CONST& rcOld, CPoint CONST& ptPivot);

    // Checking if rect1 is inside rect2
    // When rect1's either dimension (horizontal or vertical) is less or equal than that of rect2, it must stay inside;
    // Else, rect2 must stay in rect1 on the same dimension
    BOOL CRect1InsideCRect2(CRect CONST& rect1, CRect CONST& rect2);
};

//
// Other functions
//

// Use WinAPI to print unicode characters in console
VOID PrintTf(TCHAR CONST* fmt, ...);
