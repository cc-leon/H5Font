#pragma once

#ifndef __AFXWIN_H__
    #error "include 'pch.h' before including this file for PCH"
#endif

// CH5FontApp:
// See H5Font.cpp for the implementation of this class
//

class CH5FontApp : public CWinApp {
protected:
    HINSTANCE m_hResourceInst;

public:
    CH5FontApp() noexcept;
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    DECLARE_MESSAGE_MAP()
};

extern CH5FontApp theApp;
