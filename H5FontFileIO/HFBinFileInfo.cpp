#include "pch.h"
#include "HFBinFileInfo.h"
#include "pugixml.hpp"
#include "ZipFile.h"

using namespace pugi;

TCHAR CONST TARGET_PAK[] = _T("data\\texts.pak");
TCHAR CONST FALLBACK_PAK[] = _T("data\\data.pak");

size_t GetXMLBufferFromFile(LPCTSTR szZipFile, LPCTSTR szXMLFile, LPVOID lpBuffer) {
    if (!file::FileExists(szZipFile)) {
        return 0;
    }
    ZipArchive::Ptr zipFP = ZipFile::Open("123123");
}

HFBinFileInfo::HFBinFileInfo(LPCTSTR szTargetFolder)
    : m_aszBinFileNames{NULL}, m_aszHeaderNames{NULL}, m_sTargetFolder(szTargetFolder) {

    if (m_sTargetFolder[m_sTargetFolder.GetLength() - 1] != _T('\\')) {
        m_sTargetFolder.AppendChar(_T('\\'));
    }
}

HFBinFileInfo::~HFBinFileInfo() {
}

BOOL HFBinFileInfo::CheckLegal() {
    CString sTemp;
    return FALSE;
}
