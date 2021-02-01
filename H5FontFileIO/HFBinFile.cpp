#include "pch.h"
#include "HFBinFile.h"

HFBinFile::HFBinFile()
    : m_lpBuf(NULL), m_cbBuf(0), m_aciChars(NULL), m_cciChars(0) {}

HFBinFile::~HFBinFile() {}

VOID HFBinFile::InitializeInstance(LPBYTE lpBuffer, SIZE_T cbBuffer, LPCTSTR szBinFile) {
    m_sBinFile = szBinFile;
    m_lpBuf = (LPBYTE)mem::GetMem(cbBuffer);
    m_cbBuf = cbBuffer;
    ::CopyMemory(m_lpBuf, lpBuffer, cbBuffer);
}

CString CONST& HFBinFile::GetBinUID() CONST {
    return m_sBinFile;
}

BOOL HFBinFile::CreateBinFile(LPCTSTR szBinFilename) {
    return FALSE;
}

BOOL HFBinFile::CreateTxtFile(LPCTSTR szBinFilename) {
    return FALSE;
}

VOID HFBinFile::Replace(LPCUNICODEINFO uiInfo, size_t cuiInfo) {
    return;
}

VOID HFBinFile::Cleanup() {
    if (m_lpBuf != NULL) {
        mem::FreeMem((LPVOID)m_lpBuf);
        m_lpBuf = NULL;
    }
    m_cbBuf = 0;

    if (m_aciChars != NULL) {
        mem::FreeMem((LPVOID)m_aciChars);
        m_aciChars = 0;
    }
    m_cciChars = 0;

    m_sBinFile = "";
}