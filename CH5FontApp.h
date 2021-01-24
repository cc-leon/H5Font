
// H5Font.h : main header file for the H5Font application
//
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CH5FontApp:
// See H5Font.cpp for the implementation of this class
//

class CH5FontApp : public CWinApp {
public:
    CH5FontApp() noexcept;


// Overrides
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

// Implementation

public:
    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()
};

extern CH5FontApp theApp;
