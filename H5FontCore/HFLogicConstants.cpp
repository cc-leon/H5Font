#include "pch.h"
#include "HFLogicConstants.h"

namespace HFLC {
    namespace header {
        int LPSTR_TO_CODE(LPCTSTR szHeader) {
            for (int i = 0; i < HEADER_COUNT; i++) if (CString(szHeader) == CODE_TO_LPTSTR[i]) return i;
            return -1;
        }
    }
}