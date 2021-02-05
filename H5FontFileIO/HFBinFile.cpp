#include "pch.h"
#include "HFBinFile.h"

HFBinFile::HFBinFile()
    : m_lpBuf(NULL), m_cbBuf(0), m_auiChars(NULL), m_cuiChars(0),
    m_lpHead(NULL), m_cbHead(0), m_lpMid(NULL), m_cbMid(0), m_lpTail(NULL), m_cbTail(0) {}

HFBinFile::~HFBinFile() {
    Cleanup();
}

VOID HFBinFile::InitializeInstance(LPBYTE lpBuffer, SIZE_T cbBuffer, LPCTSTR szBinFile, BOOL bClone) {
    CString sLog;
    Cleanup();

    m_sBinFile = szBinFile;
    m_cbBuf = cbBuffer;
    if (bClone) {
        m_lpBuf = mem::GetMem<BYTE>(cbBuffer);
        ::CopyMemory(m_lpBuf, lpBuffer, cbBuffer);
    }
    else {
        m_lpBuf = lpBuffer;
    }

    LPDWORD lpdwReader = LPDWORD(m_lpBuf);
    LPDWORD lpdwMarker = NULL;
    for (UINT i = 0; i < m_cbBuf / sizeof(DWORD); i++) {
        lpdwReader++;
        if (HIWORD(*lpdwReader) == HFFC::bin::UNICODE_FLAG) {
            m_cuiChars++;
            if (m_lpHead == NULL) {
                m_lpHead = m_lpBuf;
                m_cbHead = i * sizeof(DWORD);
            }
            lpdwMarker = lpdwReader + 1;
        }
    }
    m_auiChars = mem::GetMem<UNICODEINFO>(m_cuiChars);

    if (lpdwMarker != NULL) {
        m_lpMid = (LPBYTE)lpdwMarker;
    }

    lpdwReader = LPDWORD(m_lpBuf);
    for (UINT i = 0; i < m_cbBuf / sizeof(DWORD); i++) {
        lpdwReader++;
        if (HIWORD(*lpdwReader) == HFFC::bin::UNICODE_FLAG) {
            m_auiChars[i].wcUnicode = LOWORD(*lpdwReader);
        }
    }

    SIZE_T stLimit = (m_cbBuf - (m_lpMid - m_lpBuf)) / sizeof(WORD);
    LPWORD lpwReader = LPWORD(m_lpMid);
    SIZE_T cciChars = 0;
    for (UINT i = 0; i < stLimit; i++) {
        if (*lpwReader == HFFC::bin::POSITION_FLAG) {
            if (m_cbMid == 0) {
                m_cbMid = (LPBYTE)lpwReader - m_lpMid;
            }

            INT32* lpi32Reader = (INT32*)(lpwReader + 1);
            for (INT j = 0; j < UNICODEINFO::POS_COUNT; j++) {
                m_auiChars[cciChars].aiPos[j] = *(lpi32Reader + j);
                lpwReader += sizeof(DWORD) / sizeof(WORD);
            }
            cciChars++;
            m_lpTail = LPBYTE(lpwReader + 1);
        }
        lpwReader++;
    }
    m_cbTail = m_cbBuf - (m_lpTail - m_lpBuf);
    m_cbTail;
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
    mem::FreeMem(m_lpBuf);
    m_cbBuf = 0;

    mem::FreeMem(m_auiChars);
    m_cuiChars = 0;

    mem::FreeMem(m_lpHead);
    m_cbHead = 0;

    mem::FreeMem(m_lpMid);
    m_cbMid = 0;

    mem::FreeMem(m_lpTail);
    m_cbTail = 0;

    m_sBinFile = _T("");
}