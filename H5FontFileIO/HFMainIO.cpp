#include "pch.h"
#include "HFMainIO.h"
#include "pugixml.hpp"
#include "ZipFile.h"

using namespace pugi;

HFMainIO::HFMainIO(LPCTSTR szTargetFolder) {
    xml_node a;
    a.end();
    ZipFile::AddFile("1.zip", "kk.png");
}

HFMainIO::~HFMainIO() {
}

BOOL HFMainIO:: CheckLegal() {
    return FALSE;
}

VOID HFMainIO::GetBinFilesInfo(std::vector<CString>& asFiles, std::vector<CString>& asBinNames) {
}

VOID HFMainIO::GetBinFiles() {

}