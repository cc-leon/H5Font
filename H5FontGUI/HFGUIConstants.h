#pragma once

namespace UIConst {
    namespace  ConfigWindow {
        int const ID = 0x1000;

        CRect const Size(0, 0, 300, 200);

        namespace UISession {
            CRect const grpRect(10, 5, 230, 200);
            CRect const lblSessionRect(10, 25, 80, 40);
            CRect const txtSessionRect(85, 23, 210, 42);
            CRect const lblClientsRect(10, 50, 210, 65);
            CRect const lstClientsRect(10, 70, 210, 120);
            CRect const optSaveRect(10, 130, 210, 145);
            CRect const optLoadRect(10, 150, 210, 165);
            CRect const optNoActionRect(10, 170, 210, 185);
        }
    }

    namespace LogWindow {
        CRect const Size(0, 200, 400, 1080);
        int const ID = 0x2000;
        enum {
            ID_txtLog = UIConst::LogWindow::ID + 1,
        };
        TCHAR CONST szLogFilename[] = _T("outputs/GUILog.rtf");
    }

    namespace DrawWindow {
        CRect const Size(400, 0, 1850, 1080);

        int const ID = 0x3000;

        enum {
            ID_container = ID + 1,
            ID_infoBar,
            ID_toolBar,
        };

        namespace Container {
            TCHAR CONST szDrawWindowContainerName[] = _T("HFDrawWindowContainer");
        }

        namespace InfoBar{
            enum {
                PANE1_PERCENTAGE = 20,
                PANE2_PERCENTAGE = 20,
                PANE3_PERCENTAGE = 20,
            };
        }
    }

    namespace Color {
        COLORREF CONST BLACK = RGB(0, 0, 0);
        COLORREF CONST WHITE = RGB(255, 255, 255);
        COLORREF CONST RED = RGB(255, 0, 0);
        COLORREF CONST GREEN = RGB(0, 255, 0);
        COLORREF CONST BLUE = RGB(0, 0, 255);
        COLORREF CONST YELLOW = RGB(255, 255, 0);
        COLORREF CONST AQUA = RGB(0, 255, 255);
        COLORREF CONST FUCHSIA = RGB(255, 0, 255);
    }

    static struct __font {
        __font();
        CFont LOG_FONT;
    } Font;

    namespace WindowMessage {
        enum {
            //
            // Logging window messages
            //



            //
            // Main window messages
            //

            // The log window is closed
            MENU_WINDOWS_LOG = WM_APP + 0x0010,

            // The draw window is closed
            MENU_WINDOWS_DRAW,
        };
    }
}