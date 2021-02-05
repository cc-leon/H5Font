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

    LPDWORD lpdwMarker = NULL;
    for (LPDWORD lpdwReader = LPDWORD(m_lpBuf); lpdwReader < LPDWORD(m_lpBuf + m_cbBuf); lpdwReader++) {
        if (HIWORD(*lpdwReader) == HFFC::bin::UNICODE_FLAG) {
            m_cuiChars++;
            if (m_lpHead == NULL) {
                m_lpHead = m_lpBuf;
                m_cbHead = (lpdwReader - (LPDWORD)m_lpBuf) * sizeof(DWORD);
            }
            lpdwMarker = lpdwReader + 1;
        }
    }
    m_auiChars = mem::GetMem<UNICODEINFO>(m_cuiChars*10);

    if (lpdwMarker != NULL) {
        m_lpMid = (LPBYTE)lpdwMarker;
    }

    SIZE_T cciChars = 0;
    LPDWORD lpdwReader = LPDWORD(m_lpBuf);
    for (LPDWORD lpdwReader = LPDWORD(m_lpBuf); lpdwReader < LPDWORD(m_lpBuf + m_cbBuf); lpdwReader++) {
        if (HIWORD(*lpdwReader) == HFFC::bin::UNICODE_FLAG) {
            m_auiChars[cciChars].wcUnicode = LOWORD(*lpdwReader);
            cciChars++;
        }
    }

    cciChars = 0;
    for (LPWORD lpwReader = LPWORD(m_lpMid); lpwReader < LPWORD(m_lpBuf + m_cbBuf);) {
        if (*lpwReader == HFFC::bin::POSITION_FLAG) {
            if (m_cbMid == 0) {
                m_cbMid = (LPBYTE)lpwReader - m_lpMid;
            }

            INT32* lpi32Reader = (INT32*)(lpwReader + 1);
            for (INT j = 0; j < UNICODEINFO::POS_COUNT; j++) {
                m_auiChars[cciChars].aiPos[j] = *(lpi32Reader + j);
                lpwReader = (LPWORD)(lpi32Reader + j + 1);
            }
            cciChars++;
            m_lpTail = (LPBYTE)lpwReader;
        }
        else {
            lpwReader++;
        }
    }
    m_cbTail = m_cbBuf - (m_lpTail - m_lpBuf);
}

CString CONST& HFBinFile::GetBinUID() CONST {
    return m_sBinFile;
}

BOOL HFBinFile::CreateBinFile(LPCTSTR szBinFilename) {
    CFile cfWriter(szBinFilename, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary);
    cfWriter.Write(m_lpHead, m_cbHead);

    for (size_t i = 0; i < m_cuiChars; i++) {
        DWORD dwToWrite = MAKELONG(m_auiChars[i].wcUnicode, HFFC::bin::UNICODE_FLAG);
        cfWriter.Write(&dwToWrite, sizeof(DWORD));
    }

    cfWriter.Write(m_lpMid, m_cbMid);

    for (size_t i = 0; i < m_cuiChars; i++) {
        cfWriter.Write(&HFFC::bin::POSITION_FLAG, sizeof(WORD));
        for (int j = 0; j < UNICODEINFO::POS_COUNT; j++) {
            cfWriter.Write(&m_auiChars[i].aiPos[j], sizeof(INT));
        }
    }

    cfWriter.Write(m_lpTail, m_cbTail);
    return FALSE;
}

BOOL HFBinFile::CreateTxtFile(LPCTSTR szBinFilename) {
    return FALSE;
}

VOID HFBinFile::Replace(LPCUNICODEINFO uiInfo, size_t cuiInfo) {
    mem::FreeMem(m_auiChars);
    m_auiChars = mem::GetMem<UNICODEINFO>(cuiInfo);
    m_cuiChars = cuiInfo;
    for (size_t i = 0; i < cuiInfo; i++) {
        m_auiChars[i] = uiInfo[i];
    }
    return;
}

VOID HFBinFile::Cleanup() {
    mem::FreeMem(m_lpBuf);
    m_cbBuf = 0;

    mem::FreeMem(m_auiChars);
    m_cuiChars = 0;

    m_sBinFile = _T("");
}