#pragma once
#include <vector>

class HFMainIO {
protected:
    CString m_sTargetFolder;
    CString m_sTargetFile;

public:
    HFMainIO(LPCTSTR szTargetFolder);
    ~HFMainIO();
    HFMainIO() = delete;
    HFMainIO(HFMainIO CONST&) = delete;
    HFMainIO(HFMainIO &&) = delete;

    BOOL CheckLegal();
    VOID GetBinFilesInfo(std::vector<CString> & asFiles, std::vector<CString>& asBinNames);
    VOID GetBinFiles();
};

