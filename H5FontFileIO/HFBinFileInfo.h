#pragma once


class HFBinFileInfo {
public:
    static size_t CONST N_FILES = 9;

    HFBinFileInfo(LPCTSTR szTargetFolder);
    ~HFBinFileInfo();

    HFBinFileInfo() = delete;
    HFBinFileInfo(HFBinFileInfo CONST&) = delete;
    HFBinFileInfo(HFBinFileInfo &&) = delete;

    BOOL InitializeInstance();
    BOOL CheckLegal();
    LPCTSTR* CONST GetHeaderNames();
    LPCTSTR* CONST GetBinFileNames();

protected:
    LPTSTR m_sTargetFolder;
    LPTSTR m_sTargetFile;
    LPTSTR m_aszHeaderNames[N_FILES];
    LPTSTR m_aszBinFileNames[N_FILES];

};
