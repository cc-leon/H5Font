#include "pch.h"
#include "HFLogLib.h"

namespace logger {
    VOID LOGMSG::SetMsg(LPCTSTR szSource) {
        cchMsg = str::GetTStrLen(szSource, LogicConst::MAX_CHARS);
        size_t cchSize = this->cchMsg + 2;
        if (szMsg != NULL) {
            mem::FreeMem(szMsg);
            szMsg = NULL;
        }
        szMsg = (LPTSTR)mem::GetMem(sizeof(TCHAR) * cchSize);
        if (::StringCchCopy(szMsg, LogicConst::MAX_CHARS, szSource) == S_OK) {
            szMsg[cchMsg + 1] = 0;
        }
    }

    LOGMSG::~LOGMSG() {
        if (szMsg != NULL) {
            mem::FreeMem(szMsg);
        }
    }
}