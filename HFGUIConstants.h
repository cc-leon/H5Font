#pragma once

namespace UIConst {
    namespace  ConfigWindow {
        int const ID = 0x1000;

        enum {
            ID_txtPipe = ID + 1,
        };

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

        namespace UIInfo {
            CRect const grpRect(240, 5, 575, 155);
            CRect const lblTLERect(10, 25, 327, 40);
            CRect const txtTLE1Rect(10, 45, 327, 64);
            CRect const txtTLE2Rect(10, 69, 327, 88);
            CRect const lblTorqueRect(10, 98, 327, 113);
            CRect const lblTorquexRect(10, 120, 20, 135);
            CRect const txtTorquexRect(25, 118, 90, 137);
            CRect const lblTorqueyRect(95, 120, 105, 135);
            CRect const txtTorqueyRect(110, 118, 175, 137);
            CRect const lblTorquezRect(180, 120, 190, 135);
            CRect const txtTorquezRect(195, 118, 260, 137);
            CRect const btnTorqueRect(265, 117, 330, 138);
        }

        namespace UILogWindow {
            CRect const grpRect(10, 210, 230, 270);
            CRect const btnCleanRect(10, 24, 100, 46);
            CRect const btnPauseRect(105, 24, 200, 46);
        }

        namespace UIDir {
            CRect const grpRect(240, 165, 575, 270);
            CRect const lblReplayRect(10, 25, 50, 40);
            CRect const txtReplayRect(55, 23, 270, 42);
            CRect const btnReplayBrowseRect(275, 22, 330, 43);
            CRect const lblLogRect(10, 50, 50, 65);
            CRect const txtLogRect(55, 48, 270, 67);
            CRect const btnLogBrowseRect(275, 47, 330, 68);
        }

        namespace UIPlayCtrl {
            CRect const grpRect(10, 280, 575, 380);
            CRect const btnRunRect(10, 24, 75, 45);
            CRect const btnPauseRect(80, 24, 145, 45);
            CRect const btnStopRect(150, 24, 215, 45);
            CRect const lblScalingRect(240, 27, 310, 42);
            CRect const txtScalingRect(315, 25, 390, 44);
            CRect const btnScalingRect(395, 24, 470, 45);
            CRect const sldPlayRect(10, 55, 555, 75);
            CRect const lblFirstTimeRect(10, 75, 90, 90);
            CRect const lblCurrTimeRect(250, 75, 320, 90);
            CRect const lblLastTimeRect(490, 75, 555, 90);
        }
    }

    namespace LogWindow {
        CRect const Size(0, 200, 400, 1080);
        int const ID = 0x2000;
    }

    namespace DrawWindow {
        CRect const Size(400, 0, 1850, 1080);
        int const ID = 0x3000;
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

    namespace WindowMessage {
        enum {
            //
            // Logging window messages
            //

            // For show a rich formated text, send from Logger singleton
            // LPARAM is a LOGMSG structure pointer that contains all formatting information,
            // WPARAM is NULL. Sender handles cleanup
            LOG_SHOW_TEXT = WM_APP + 0x0001,

            // To save the log into a file, send from Logger singleton
            // LPARAM is a LPTSTR pointer that contains filename,
            // WPARAM is NULL. Sender handles cleanup
            LOG_SAVE_TEXT,
        };
    }
}