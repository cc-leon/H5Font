#include "pch.h"

#include <sstream>
#include <fstream>

#include "HFFileUtils.h"
#include "HFBinFilesInfo.h"
#include "pugixml.hpp"

using namespace pugi;

HFBinFilesInfo::HFBinFilesInfo() {}

HFBinFilesInfo::~HFBinFilesInfo() {}

BOOL HFBinFilesInfo::CheckLegal(LPCTSTR szPakName) {
    CString sCmd;
    sCmd.Format(_T("%s x %s -y -o%s"), HFFC::exe::ZIP_CMD, szPakName, HFFC::pak::TEMP_FOLDER);
    CString sLog;

    for (int i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        CString sParam;
        sParam.Format(_T(" %s\\%s.%s"), HFFC::pak::XML_PATH, HFLC::header::CODE_TO_LPTSTR[i], HFFC::pak::XML_BININFO);
        sCmd += sParam;
    }

    DWORD dwExitCode;
    CString sStdOut = sys::RunExe(sCmd, &dwExitCode);
    LOG.log(HFSTRC(IDS_LOG_NOW_EXECUTING), LOG.STD, TRUE);
    LOG.log(sCmd, LOG.STD);
    LOG.log(sStdOut, LOG.STD);

    if (dwExitCode) {
        sLog.Format(HFSTRC(IDS_LOG_WRONG_EXIT_CODE), dwExitCode);
        LOGUSR(sLog);
        return FALSE;
    }

    CString asUIDs[HFLC::header::HEADER_COUNT];
    for (int i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        CString sXmlFile;
        sXmlFile.Format(
            _T("%s\\%s\\%s.%s"),
            HFFC::pak::TEMP_FOLDER,
            HFFC::pak::XML_PATH,
            HFLC::header::CODE_TO_LPTSTR[i],
            HFFC::pak::XML_BININFO);

        if (!file::FileExists(sXmlFile)) {
            sLog.Format(HFSTRC(IDS_LOG_FILE_NOT_FOUND), sXmlFile);
            LOGUSR(sLog);
            return FALSE;
        }

        std::ifstream xml_fp(str::CString2STDString(sXmlFile), std::ifstream::in);
        xml_document xmlDoc;
        xml_parse_result xmlParserResult = xmlDoc.load(xml_fp);
        if (xmlParserResult.status != xml_parse_status::status_ok) {
            sLog.Format(HFSTRC(IDS_LOG_NOT_PROPER_XML), sXmlFile);
            LOGUSR(sLog);
            return FALSE;
        }
        std::string uid = "";
        for (xml_node fontNode = xmlDoc.child(HFFC::xml::XML_FONT_NODE); fontNode; fontNode = xmlDoc.next_sibling(HFFC::xml::XML_FONT_NODE)) {
            uid = fontNode.child_value(HFFC::xml::XML_UID_SUBNODE);
            if (uid != "") {
                break;
            }
        }
        if (uid == "") {
            sLog.Format(HFSTRC(IDS_LOG_XML_UID_NOT_FOUND), sXmlFile);
            LOGUSR(sLog);
            return FALSE;
        }
        xml_fp.close();

        if (!::DeleteFile(sXmlFile)) {
            sLog.Format(HFSTRC(IDS_LOG_DELETING_TEMP_ERROR), sXmlFile);
            LOGUSR(sLog);
            return FALSE;
        }
        asUIDs[i] = str::STDString2CString(uid);
        sLog.Format(HFSTRC(IDS_LOG_HEADER_BIN_MAPPING), sXmlFile, asUIDs[i]);
        LOG.log(sLog, LOG.INFO);
    }

    sCmd.Format(_T("%s x %s -y -o%s"), HFFC::exe::ZIP_CMD, szPakName, HFFC::pak::TEMP_FOLDER);

    for (int i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        CString sParam;
        sParam.Format(_T(" %s\\%s"), HFFC::pak::BIN_PATH, (LPCTSTR)asUIDs[i]);
        sCmd += sParam;
    }

    sStdOut = sys::RunExe(sCmd, &dwExitCode);
    LOG.log(HFSTRC(IDS_LOG_NOW_EXECUTING), LOG.STD, TRUE);
    LOG.log(sCmd, LOG.STD);
    LOG.log(sStdOut, LOG.STD);

    if (dwExitCode) {
        sLog.Format(HFSTRC(IDS_LOG_WRONG_EXIT_CODE), dwExitCode);
        LOGUSR(sLog);
        return FALSE;
    }
    for (int i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        CString sBinFile;
        sBinFile.Format(_T("%s\\%s\\%s"), HFFC::pak::TEMP_FOLDER, HFFC::pak::BIN_PATH, (LPCTSTR)asUIDs[i]);
        if (!file::FileExists(sBinFile)) {
            sLog.Format(HFSTRC(IDS_LOG_FILE_NOT_FOUND), dwExitCode);
            LOGUSR(sLog);
            return FALSE;
        }

        CFile cfBinFile(sBinFile, CFile::modeRead | CFile::typeBinary);
        LPBYTE lpBuf = mem::GetMem<BYTE>((SIZE_T)cfBinFile.GetLength());
        if (cfBinFile.Read(lpBuf, (UINT)cfBinFile.GetLength())) {
            sLog.Format(HFSTRC(IDS_LOG_BIN_IN_MEMORY), sBinFile);
            LOG.log(sLog, LOG.INFO);
        }
        else {
            sLog.Format(HFSTRC(IDS_LOG_BIN_FILE_ERROR), sBinFile);
            LOGUSR(sLog);
            return FALSE;
        }
        m_aBinfiles[i].InitializeInstance(lpBuf, (SIZE_T)cfBinFile.GetLength(), asUIDs[i]);
    }

    return TRUE;
}

HFBinFile& HFBinFilesInfo::operator [] (size_t iIndex) {
    return m_aBinfiles[iIndex];
}

VOID HFBinFilesInfo::Cleanup() {
    m_sTargetFile = _T("");
    file::ClearFolder(HFFC::pak::TEMP_FOLDER);
    file::CreateFolderIfNotExists(HFFC::pak::TEMP_FOLDER);
}

BOOL HFBinFilesInfo::InitializeInstance(LPCTSTR szTargetName) {
    Cleanup();
    if (file::FileExists(szTargetName) && CheckLegal(szTargetName)) {
        m_sTargetFile = szTargetName;
        return TRUE;
    }
    else {
        return FALSE;
    }
}

BOOL HFBinFilesInfo::SyncFromDrawCentre(HFDrawDCsCentre* dcDrawCentre) {
    for (int i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        size_t cuiNew = (*dcDrawCentre)[i].GetUnicodeCount();
        if (!cuiNew) {
            return FALSE;
        }

        LPUNICODEINFO auiNew = mem::GetMem<UNICODEINFO>(cuiNew);
        for (size_t j = 0; j < cuiNew; j++) {
            (*dcDrawCentre)[i].FillUNICODEINFO(j, &auiNew[j]);
        }
        m_aBinfiles[i].Replace(auiNew, cuiNew);
        mem::FreeMem(auiNew);
    }

    return TRUE;
}

BOOL HFBinFilesInfo::SaveAll() {
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

BOOL HFBinFilesInfo::SaveOneByOne(int& i) {
    if (i >= HFLC::header::HEADER_COUNT) {
        return TRUE;
    }

    CString sBinFile;
    sBinFile.Format(
        _T("%s\\%s\\%s"),
        HFFC::pak::TEMP_FOLDER,
        HFFC::pak::BIN_PATH,
        (LPCTSTR)m_aBinfiles[i].GetBinUID());
    if (!m_aBinfiles[i].CreateBinFile(sBinFile)) {
        return FALSE;
    }

    i++;

    return TRUE;
}
