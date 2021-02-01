#include "pch.h"
#include "HFFileUtils.h"


namespace str {

    CString STDString2CString(std::string CONST& inputString) {
        return CString(CStringA2CString(inputString.c_str()));
    }

    std::string CString2STDString(CString CONST& inputString) {
        return std::string(CString2CStringA(inputString));
    }

}