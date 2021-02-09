#include "pch.h"
#include "HFBinFile.h"

HFBinFile::HFBinFile()
    : m_lpBuf(NULL), m_cbBuf(0), m_auiChars(NULL), m_cuiChars(0),
    m_lpHead(NULL), m_cbHead(NULL), m_lpMid(NULL), m_cbMid(0), m_lpTail(NULL), m_cbTail(0) {}

HFBinFile::~HFBinFile() {
    Cleanup();
}

BOOL HFBinFile::InitializeInstance(LPBYTE lpBuffer, SIZE_T cbBuffer, LPCTSTR szBinFile, BOOL bClone) {
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

    m_lpHead = m_lpBuf;

    LPWORD lpwReader = (LPWORD)m_lpBuf;
    while (*lpwReader != HFFC::bin::UNICODE_FLAG && lpwReader < LPWORD(m_lpBuf + m_cbBuf)) {
        lpwReader++;
    }
    m_cbHead = (LPBYTE)lpwReader - m_lpBuf;

    LPDWORD lpdwReader = (LPDWORD)(lpwReader);
    m_cuiChars = 0;
    while (LOWORD(*lpdwReader) == HFFC::bin::UNICODE_FLAG) {
        m_cuiChars++; lpdwReader++;
    }
    m_auiChars = mem::GetMem<UNICODEINFO>(m_cuiChars);

    lpdwReader = (LPDWORD)(m_lpBuf + m_cbHead);
    size_t cuiChars = 0;
    while (LOWORD(*lpdwReader) == HFFC::bin::UNICODE_FLAG) {
        m_auiChars[cuiChars].wcUnicode = HIWORD(*lpdwReader);
        cuiChars++; lpdwReader++;
    }
    if (cuiChars != m_cuiChars) {
        sLog.Format(HFSTRC(IDS_LOG_BIN_DATA_ERROR), m_sBinFile);
        LOGUSR(sLog);
        return FALSE;
    }

    m_lpMid = (LPBYTE)lpdwReader;
    lpwReader = (LPWORD)lpdwReader;
    while (*lpwReader != HFFC::bin::POSITION_FLAG && lpwReader < LPWORD(m_lpBuf + m_cbBuf)) {
        lpwReader++;
    }
    m_cbMid = (LPBYTE)lpwReader - m_lpMid;

    cuiChars = 0;
    while (*lpwReader == HFFC::bin::POSITION_FLAG) {
        INT32* lpi32Reader = (INT32*)(lpwReader + 1);
        for (INT j = 0; j < UNICODEINFO::POS_COUNT; j++) {
            m_auiChars[cuiChars].aiPos[j] = *(lpi32Reader + j);
        }

        lpwReader = (LPWORD)(lpi32Reader + UNICODEINFO::POS_COUNT );
        cuiChars++;
    }
    m_lpTail = (LPBYTE)lpwReader;
    m_cbTail = m_cbBuf - (m_lpTail - m_lpBuf);
    if (cuiChars != m_cuiChars) {
        sLog.Format(HFSTRC(IDS_LOG_BIN_DATA_ERROR), m_sBinFile);
        LOGUSR(sLog);
        return FALSE;
    }
    LPUNICODEINFO* lpMask = mem::GetMem<LPUNICODEINFO>(0xFFFF);
    ::ZeroMemory(lpMask, sizeof(LPUNICODEINFO) * (0XFFFF));
    for (size_t i = 0; i < m_cuiChars; i++) {
        lpMask[m_auiChars[i].wcUnicode] = &m_auiChars[i];
    }

    LPUNICODEINFO auiNew = mem::GetMem<UNICODEINFO>(m_cuiChars);
    size_t j = 0;
    for (size_t i = 0; i < 0XFFFF; i++) {
        if (lpMask[i] != NULL) {
            auiNew[j++] = *lpMask[i];
        }
    }

    mem::FreeMem(lpMask);
    m_auiChars = auiNew;
    sLog.Format(HFSTRC(IDS_LOG_BIN_DATA_SUCCESS), m_sBinFile);
    LOG.log(sLog, LOG.INFO);
    return TRUE;
}

CString CONST& HFBinFile::GetBinUID() CONST {
    return m_sBinFile;
}

BOOL HFBinFile::CreateBinFile(LPCTSTR szBinFilename) {
    CString sLog;
    if (!m_cuiChars || !m_auiChars || !m_lpBuf || !m_cbBuf) {
        sLog.Format(HFSTRC(IDS_LOG_WRITE_BIN_ERROR), szBinFilename);
        LOGUSR(sLog);
        return FALSE;
    }

    CFile cfWriter(szBinFilename, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary);
    cfWriter.Write(m_lpHead, m_cbHead);

    for (size_t i = 0; i < m_cuiChars; i++) {
        DWORD dwToWrite = MAKELONG(HFFC::bin::UNICODE_FLAG, m_auiChars[i].wcUnicode);
        cfWriter.Write(&dwToWrite, sizeof(DWORD));
    }

    cfWriter.Write(m_lpMid, m_cbMid);

    for (size_t i = 0; i < m_cuiChars; i++) {
        cfWriter.Write(&HFFC::bin::POSITION_FLAG, sizeof(WORD));
        for (int j = 0; j < UNICODEINFO::POS_COUNT; j++) {
            cfWriter.Write(&(m_auiChars[i].aiPos[j]), sizeof(INT32));
        }
    }

    cfWriter.Write(m_lpTail, m_cbTail);
    sLog.Format(HFSTRC(IDS_LOG_BIN_WRITE_SUCCESS), szBinFilename);
    LOG.log(sLog, LOG.INFO);
    return TRUE;
}

BOOL HFBinFile::CreateTxtFile(LPCTSTR szTxtFilename) {
    TCHAR cSeparator = _T('\t');
    TCHAR cNewLine[] = _T("\r\n");
    INT cchTextBuffer = 0;
    CString sBuffer;
    CFile cf(szTxtFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

    for (SIZE_T i = 0; i < m_cuiChars; i++) {
        sBuffer.Format(_T("%04x"), m_auiChars[i].wcUnicode);
        LPBYTE abBuffer = (LPBYTE)(LPCTSTR)sBuffer;
        size_t cchBuffer = sBuffer.GetLength() * sizeof(TCHAR);
        cf.Write(abBuffer, cchBuffer);
        cf.Write((LPVOID)&cSeparator, sizeof(TCHAR));

        sBuffer = "";
        sBuffer.AppendChar(m_auiChars[i].wcUnicode);
        sBuffer.Format(_T("\'%s\'"), (LPCTSTR)sBuffer);
        abBuffer = (LPBYTE)(LPCTSTR)sBuffer;
        cchBuffer = sBuffer.GetLength() * sizeof(TCHAR);
        cf.Write(abBuffer, cchBuffer);
        cf.Write((LPVOID)&cSeparator, sizeof(TCHAR));

        for (INT j = 0; j < UNICODEINFO::POS_COUNT; j++) {
            sBuffer.Format(_T("%04d"), m_auiChars[i].aiPos[j]);
            abBuffer = (LPBYTE)(LPCTSTR)sBuffer;
            cchBuffer = sBuffer.GetLength() * sizeof(TCHAR);
            cf.Write(abBuffer, cchBuffer);

            if (j < UNICODEINFO::POS_COUNT - 1) {
                cf.Write((LPVOID)&cSeparator, sizeof(TCHAR));
            }
        }
        cf.Write((LPVOID)cNewLine, sizeof(TCHAR) * 2);
    }

    return FALSE;
}

VOID HFBinFile::Replace(LPCUNICODEINFO uiInfo, size_t cuiInfo) {
    int* pCharCount = NULL;
    for (size_t i = 0; i < HFFC::bin::HEAD_COUNTS_NUM; i++) {
        pCharCount = (int*)(m_lpHead + HFFC::bin::HEAD_COUNTS_ADDR[i]);
        *pCharCount = HFFC::bin::HEAD_COUNTS_FACTOR[i] * cuiInfo + HFFC::bin::HEAD_COUNTS_OFFSET[i];
    }

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
