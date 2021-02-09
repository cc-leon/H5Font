#pragma once

namespace str {
    // Convert std string to CString
    CString STDString2CString(std::string CONST& inputString);

    // Convert CString to std string
    std::string CString2STDString(CString CONST& inputString);
}
