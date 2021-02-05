#include "pch.h"

#include <sstream>
#include <fstream>

#include "HFFileUtils.h"
#include "HFBinFilesInfo.h"
#include "pugixml.hpp"

using namespace pugi;

HFBinFilesInfo::HFBinFilesInfo() {
    file::CreateFolderIfNotExists(HFFC::pak::TEMP_FOLDER);
}

HFBinFilesInfo::~HFBinFilesInfo() {}

BOOL HFBinFilesInfo::CheckLegal(LPCTSTR szPakName) {
    // Step 1. Unzip all xml files
    CString sCmd;
    sCmd.Format(_T("%s x %s -o%s"), HFFC::exe::ZIP_CMD, szPakName, HFFC::pak::TEMP_FOLDER);
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
        sXmlFile.Format(_T("%s\\%s\\%s.%s"), HFFC::pak::TEMP_FOLDER, HFFC::pak::XML_PATH, HFLC::header::CODE_TO_LPTSTR[i], HFFC::pak::XML_BININFO);
        if (!file::FileExists(sXmlFile)) {
            sLog.Format(HFSTRC(IDS_LOG_FILE_NOT_FOUND), dwExitCode);
            LOGUSR(sLog);
            return FALSE;
        }

        std::ifstream xml_fp(str::CString2STDString(sXmlFile), std::ifstream::in);
        xml_document xmlDoc;
        xml_parse_result xmlParserResult = xmlDoc.load(xml_fp);
        if (xmlParserResult.status != xml_parse_status::status_ok) {
            sLog.Format(HFSTRC(IDS_LOG_NOT_PROPER_XML), sXmlFile);
            LOG.log(sLog);
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
            LOG.log(sLog);
            return FALSE;
        }
        xml_fp.close();
        asUIDs[i] = str::STDString2CString(uid);
    }

    sCmd.Format(_T("%s x %s -o%s"), HFFC::exe::ZIP_CMD, szPakName, HFFC::pak::TEMP_FOLDER);

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
        cfBinFile.Read(lpBuf, (UINT)cfBinFile.GetLength());
        m_aBinfiles[i].InitializeInstance(lpBuf, (SIZE_T)cfBinFile.GetLength(), asUIDs[i]);
        break;
    }

    return TRUE;
}

VOID HFBinFilesInfo::Cleanup() {
    m_sTargetFile = "";
    for (int i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        m_aBinfiles[i].Cleanup();
    }
    file::ClearFolder(HFFC::pak::TEMP_FOLDER);
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
