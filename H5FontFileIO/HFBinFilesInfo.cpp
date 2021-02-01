#include "pch.h"

#include <sstream>

#include "HFFileUtils.h"
#include "HFBinFilesInfo.h"
#include "pugixml.hpp"
#include "ZipFile.h"

using namespace pugi;

HFBinFilesInfo::HFBinFilesInfo() {}

HFBinFilesInfo::~HFBinFilesInfo() {}

BOOL HFBinFilesInfo::CheckLegal(LPCTSTR szPakName) {
    ZipArchive::Ptr zipFP = ZipFile::Open(str::CString2STDString(szPakName));

    for (int i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        std::string sCurrXML = std::string(HFFC::parser::XML_PATH)
            + std::string("/")
            + str::CString2STDString(HFLC::header::CODE_TO_LPTSTR[i])
            + std::string(".")
            + std::string(HFFC::parser::XML_BININFO);

        ZipArchiveEntry::Ptr xmlEntry = zipFP->GetEntry(sCurrXML);
        if (xmlEntry == nullptr) {
            return FALSE;
        }

        std::istream* xmlStream = xmlEntry->GetDecompressionStream();
        xml_document xmlDoc;
        xml_parse_result xmlParserResult = xmlDoc.load(*xmlStream);
        if (xmlParserResult.status != xml_parse_status::status_ok) {
            return FALSE;
        }

        xmlEntry->CloseDecompressionStream();

        std::string uid = "";
        for (xml_node fontNode = xmlDoc.child(HFFC::parser::XML_FONT_NODE); fontNode; fontNode = xmlDoc.next_sibling(HFFC::parser::XML_FONT_NODE)) {
            uid = fontNode.child_value(HFFC::parser::XML_UID_SUBNODE);
            if (uid != "") {
                break;
            }
        }
        if (uid == "") {
            return FALSE;
        }

        std::string sCurrBin;
        sCurrBin = std::string(HFFC::parser::BIN_PATH) + std::string("/") + uid;
        ZipArchiveEntry::Ptr binEntry = zipFP->GetEntry(sCurrBin);
        if (binEntry == nullptr) {
            return FALSE;
        }

        std::istream* binStream = binEntry->GetDecompressionStream();
        std::vector<BYTE> vec;
        vec.insert(vec.begin(),
            std::istream_iterator<BYTE>(*binStream),
            std::istream_iterator<BYTE>());
        xmlEntry->CloseDecompressionStream();
        m_aBinfiles[i].InitializeInstance(vec.data(), vec.size(), str::STDString2CString(uid));
    }

    return TRUE;
}

VOID HFBinFilesInfo::Cleanup() {
    m_sTargetFile = "";
    for (int i = 0; i < HFLC::header::HEADER_COUNT; i++) {
        m_aBinfiles[i].Cleanup();
    }
    file::ClearFolder(HFFC::output::TEMP_FOLDER);
}

BOOL HFBinFilesInfo::InitializeInstance(LPCTSTR szTargetName) {
    if (szTargetName != NULL) {
        CString sCurrZip;
        sCurrZip.Format(_T("%s\\%s"), szTargetName, HFFC::parser::TARGET_PAK);
        if (file::FileExists(sCurrZip) && CheckLegal(sCurrZip)) {
            m_sTargetFile = sCurrZip;
            return TRUE;
        }
        Cleanup();

        sCurrZip.Format(_T("%s\\%s"), szTargetName, HFFC::parser::FALLBACK_PAK);
        if (file::FileExists(sCurrZip) && CheckLegal(sCurrZip)) {
            m_sTargetFile = sCurrZip;
            return TRUE;
        }
        Cleanup();
        return FALSE;
    }
    else {
        if (file::FileExists(szTargetName) && CheckLegal(szTargetName)) {
            m_sTargetFile = szTargetName;
            return TRUE;
        }
        else {
            Cleanup();
            return FALSE;
        }
    }

}
