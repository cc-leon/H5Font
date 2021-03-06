#pragma once
#include "HFBinFile.h"
#include "../H5FontCore/HFDrawDCsCentre.h"

class HFBinFilesInfo {
public:
    static int const MAX_HEADER_LEN = 10;
    static int const MAX_UID_LEN = 40;

    HFBinFilesInfo();
    ~HFBinFilesInfo();

    HFBinFilesInfo(HFBinFilesInfo CONST&) = delete;
    HFBinFilesInfo(HFBinFilesInfo &&) = delete;
    void operator=(HFBinFilesInfo CONST&) = delete;
    void operator=(HFBinFilesInfo &&) = delete;

    BOOL InitializeInstance(LPCTSTR szTargetName);
    BOOL SyncFromDrawCentre(HFDrawDCsCentre* dcDrawCentre);
    BOOL SaveAll();
    BOOL SaveOneByOne(int& iIndex);
    HFBinFile& operator [] (size_t iIndex);

protected:
    CString m_sTargetFile;
    HFBinFile m_aBinfiles[HFLC::header::HEADER_COUNT];

    BOOL CheckLegal(LPCTSTR szPakName);
    VOID Cleanup();
};
