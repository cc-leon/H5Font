#pragma once

namespace HFUIC {
    namespace  MainWindow {
        int const ID = 0x1000;
        enum {
            ID_btnBrowsePak = ID + 1,
            ID_txtPak,
            ID_ddlHeaderSelect,
            ID_ddlFontSelect,
            ID_txtSize,
            ID_txtPadding,
            ID_txtBold,
            ID_btnItalic,
            ID_btnUnderline,
            ID_btnRun,
            ID_btnPreviewDraw,
            ID_btnPackage,
            ID_btnOpenFolder,
        };

        CRect const Size(0, 0, 400, 400);
        int const MARGIN = 3;
        namespace UIStep1{
            CRect const grpRect(0 + MARGIN, 0 + MARGIN, 400 - MARGIN - 16, 70 - MARGIN * 2);
            CRect const lblPakRect(10, 30, 130, 50);
            CRect const txtPakRect(130, 28, 270, 52);
            CRect const btnBrowsePakRect(280, 25, 370, 55);
        }

        namespace UIStep2 {
            CRect const grpRect(0 + MARGIN, 70 + MARGIN, 400 - MARGIN * 2 - 16, 270 - MARGIN * 2);
            CRect const lblHeaderSelectRect(10, 25, 190, 45);
            CRect const ddlHeaderSelectRect(200, 22, 365, 30);
            CRect const lblSplitterRect(10, 47, 370, 65);
            CRect const lblFontSelectRect(10, 70, 100, 90);
            CRect const ddlFontSelectRect(100, 66, 270, 90);
            CRect const lblSizeRect(280, 70, 320, 90);
            CRect const txtSizeRect(320, 68, 350, 90);
            CRect const lblPaddingRect(10, 105, 75, 125);
            CRect const txtPaddingRect(75, 103, 100, 125);
            CRect const lblBoldRect(125, 105, 165, 125);
            CRect const txtBoldRect(165, 103, 195, 125);
            CRect const btnItalicRect(220, 105, 280, 125);
            CRect const btnUnderlineRect(280, 105, 340, 125);
            CRect const lblPreviewRect(10, 135, 360, 185);
        }

        namespace UIStep3 {
            CRect const grpRect(0 + MARGIN, 270, 170, 340 - MARGIN * 2);
            CRect const btnRunRect(10, 25, 80, 55);
            CRect const btnPreviewDrawRect(85, 25, 155, 55);
        }

        namespace UIStep4 {
            CRect const grpRect(180, 270, 400 - MARGIN * 2 - 16, 340 - MARGIN * 2);
            CRect const btnPackageRect(10, 25, 80, 55);
            CRect const btnOpenFolderRect(85, 25, 190, 55);
        }

    }

    namespace LogWindow {
        CRect const Size(0, 400, 400, 1080);
        int const ID = 0x2000;
        enum {
            ID_txtLog = ID + 1,
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
        CFont NORMAL_FONT;
        CFont BOLD_FONT;
    } Font;

    namespace WindowMessage {
        enum {
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