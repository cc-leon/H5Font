#pragma once

class HFBinFile {
public:
    HFBinFile();
    ~HFBinFile();
    VOID InitializeInstance(LPBYTE lpBuffer, SIZE_T cbBuffer, LPCTSTR szBinFile, BOOL bClone=FALSE);
    CString CONST& GetBinUID() CONST;
    BOOL CreateBinFile(LPCTSTR szBinFilename=NULL);
    BOOL CreateTxtFile(LPCTSTR szBinFilename=NULL);
    VOID Replace(LPCUNICODEINFO uiInfo, size_t cuiInfo);
    VOID Cleanup();

protected:
    CString m_sBinFile;
    LPBYTE m_lpBuf;
    SIZE_T m_cbBuf;
    LPUNICODEINFO m_auiChars;
    SIZE_T m_cuiChars;

    LPBYTE m_lpHead;
    SIZE_T m_cbHead;
    LPBYTE m_lpMid;
    SIZE_T m_cbMid;
    LPBYTE m_lpTail;
    SIZE_T m_cbTail;

};

