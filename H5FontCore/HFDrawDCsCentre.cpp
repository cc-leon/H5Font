#include "pch.h"
#include "HFDrawDCsCentre.h"

HFDrawDCsCentre hfcore;

HFDrawDCsCentre::HFDrawDCsCentre() {}
HFDrawDCsCentre::~HFDrawDCsCentre() {}

HFMemDC& HFDrawDCsCentre::operator [] (size_t index) {
    return m_dcs[index];
}

BOOL HFDrawDCsCentre::DrawAtOnce(LPCFONTINFO fiInfo) {
    int i = 0;
    int prev_i;

    do {
        prev_i = i;
        if (!DrawOneByOne(fiInfo, i)) {
            return FALSE;
        }
    } while (i > prev_i);

    return TRUE;
}

BOOL HFDrawDCsCentre::DrawOneByOne(LPCFONTINFO fiInfo, int& i) {
    if (i >= HFLC::header::HEADER_COUNT) {
        return TRUE;
    }

    CString sLog;
    sLog.Format(
        HFSTRC(IDS_LOG_PROCESSING_HEADER),
        HFLC::header::CODE_TO_LPTSTR[i],
        fiInfo->szFacenam,
        fiInfo->nHeight,
        fiInfo->nWeight,
        fiInfo->nPadding);
    LOG.log(_T(""));
    LOG.log(sLog, LOG.INFO);
    if (!m_dcs[i].CreateHFMemDC(*fiInfo, i)) {
        return FALSE;
    }

    i++;

    return TRUE;
}

BOOL HFDrawDCsCentre::SaveAtOnce() {
    int i = 0;
    int prev_i;

    do {
        prev_i = i;
        if (!SaveOneByOne(i)) {
            return FALSE;
        }
    } while (i > prev_i);

    return TRUE;

}

BOOL HFDrawDCsCentre::SaveOneByOne(int& i) {
    if (i >= HFLC::header::HEADER_COUNT) {
        return TRUE;
    }

    CString sTemp;
    sTemp.Format(
        _T("%s\\%s\\%s.(Texture).dds"),
        HFFC::pak::TEMP_FOLDER,
        HFFC::pak::XML_PATH,
        HFLC::header::CODE_TO_LPTSTR[i]);
    if (!m_dcs[i].SaveDDS(sTemp)) {
        return FALSE;
    }

    i++;

    return TRUE;
}
