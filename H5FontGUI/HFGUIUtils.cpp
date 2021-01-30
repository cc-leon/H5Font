#include "pch.h"
#include "HFGUIUtils.h"
#include "../H5FontCore/HFUtils.h"

namespace ui {
    CRect GetWindowCRectInParent(CWnd * pWnd) {
        CRect rcResult;
        ::GetWindowRect(*pWnd, &rcResult);
        pWnd->GetParent()->ScreenToClient(rcResult);
        return CRect(rcResult);
    }

    CRect ZoomCRectOverCPoint(CSize CONST& csNew, CRect CONST& rcOld, CPoint CONST& ptPivot) {
        CRect rcResult(rcOld);
        LONG lHDelta = csNew.cx - rcOld.Width();
        LONG lVDelta = csNew.cy - rcOld.Height();
        rcResult.left += (LONG)((FLOAT)(rcResult.left - ptPivot.x) / rcOld.Width() * lHDelta);
        rcResult.right += (LONG)((FLOAT)(rcResult.right - ptPivot.x) / rcOld.Width() * lHDelta);
        rcResult.top += (LONG)((FLOAT)(rcResult.top - ptPivot.y) / rcOld.Width() * lHDelta);
        rcResult.bottom += (LONG)((FLOAT)(rcResult.bottom - ptPivot.y) / rcOld.Width() * lHDelta);
        return rcResult;
    }

    BOOL CRect1InsideCRect2(CRect CONST& rect1, CRect CONST& rect2) {
        CRect CONST* rcSmall, *rcBig;
        rcSmall = rect1.Width() > rect2.Width() ? &rect2 : &rect1;
        rcBig = rect1.Width() > rect2.Width() ? &rect1 : &rect2;
        if (rcSmall->left < rcBig->left || rcSmall->right > rcBig->right) {
            return FALSE;
        }

        rcSmall = rect1.Height() > rect2.Height() ? &rect2 : &rect1;
        rcBig = rect1.Height() > rect2.Height() ? &rect1 : &rect2;
        if (rcSmall->top < rcBig->top || rcSmall->bottom > rcBig->bottom) {
            return FALSE;
        }

        return TRUE;
    }

    CString LoadRcString(UINT uiResourceID) {
        CStringW wsResult;
        if (wsResult.LoadString(uiResourceID) == 0) {
            return CString();
        }

#ifdef _UNICODE
            return sWResult;

#else
        TCHAR szConverted[0x400];
        ::WideCharToMultiByte(936, NULL, wsResult,-1,szConverted, 0x100, NULL, NULL);
        CString sResult(szConverted);
        return sResult;

#endif
    }

}
