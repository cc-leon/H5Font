#include "pch.h"
#include "HFBinFileInfo.h"
#include "pugixml.hpp"
#include "ZipFile.h"

using namespace pugi;

HFBinFileInfo::HFBinFileInfo(LPCTSTR szTargetFolder)
    : m_aszBinFileNames{NULL}, m_aszHeaderNames{NULL}, m_sTargetFile(NULL), m_sTargetFolder(NULL) {
    xml_node a;
    a.end();
    ZipFile::AddFile("1.zip", "kk.png");
}

HFBinFileInfo::~HFBinFileInfo() {
}

BOOL HFBinFileInfo::CheckLegal() {
    return FALSE;
}
