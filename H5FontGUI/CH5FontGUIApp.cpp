#include "pch.h"
#include "CH5FontGUIApp.h"
#include "HFMainWindow.h"
#include "HFLogWindow.h"
#include "../HFzhCNResourceDLL/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CH5FontApp, CWinApp)
END_MESSAGE_MAP()


CH5FontApp::CH5FontApp() noexcept : m_hResourceInst(NULL){
    // support Restart Manager
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
    // If the application is built using Common Language Runtime support (/clr):
    //     1) This additional setting is needed for Restart Manager support to work properly.
    //     2) In your project, you must add a reference to System.Windows.Forms in order to build.
    System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

    // TODO: replace application ID string below with unique ID string; recommended
    // format for string is CompanyName.ProductName.SubProduct.VersionInformation
    SetAppID(_T("H5Font.AppID.NoVersion"));

    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

// The one and only CH5FontApp object

CH5FontApp theApp;

// CH5FontApp initialization
BOOL CH5FontApp::InitInstance() {
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    m_hResourceInst = ::AfxLoadLibraryEx(_T("HFzhCNResourceDLL.dll"), NULL, NULL);
    ::AfxSetResourceHandle(m_hResourceInst);

    // Initialize OLE libraries
    if (!AfxOleInit()) {
        AfxMessageBox(_T("Fatal error"));
        return FALSE;
    }

    AfxEnableControlContainer();

    EnableTaskbarInteraction(FALSE);

    //AfxInitRichEdit2() is required to use RichEdit control
    AfxInitRichEdit2();

    // Standard initialization
    SetRegistryKey(_T("H5Font"));

    file::CreateFolderIfNotExists(HFFC::pak::TEMP_FOLDER);

    HFMainWindow* pMainWnd = new HFMainWindow;
    if (!pMainWnd) {
        return FALSE;
    }
    m_pMainWnd = pMainWnd;
    pMainWnd->CreateHFMainWindow();
    pMainWnd->ShowWindow(SW_SHOW);
    pMainWnd->UpdateWindow();

    return TRUE;
}

int CH5FontApp::ExitInstance() {
    AfxOleTerm(FALSE);
    return CWinApp::ExitInstance();
}
