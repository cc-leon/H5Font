#include "pch.h"
#include "HFGUIConstants.h"

namespace HFUIC {
    __font::__font() {
        LOG_FONT.CreateFont(
            20,                            // Height
            0,                             // Width
            0,                             // Escapement
            0,                             // OrientationME
            FW_SEMIBOLD,                     // Weight
            FALSE,                         // Italic
            FALSE,                          // Underline
            0,                             // StrikeOut
            ANSI_CHARSET,                  // CharSet
            OUT_DEVICE_PRECIS,            // OutPrecision
            CLIP_DEFAULT_PRECIS,           // ClipPrecision
            CLEARTYPE_QUALITY,               // Quality
            FIXED_PITCH || FF_MODERN,//FIXED_PITCH | FF_ROMAN,      // PitchAndFamily
            _T("Consolas"));      // Facename

        NORMAL_FONT.CreateFont(
            18,                            // Height
            0,                             // Width
            0,                             // Escapement
            0,                             // OrientationME
            FW_NORMAL,                     // Weight
            FALSE,                         // Italic
            FALSE,                          // Underline
            0,                             // StrikeOut
            ANSI_CHARSET,                  // CharSet
            OUT_DEVICE_PRECIS,            // OutPrecision
            CLIP_DEFAULT_PRECIS,           // ClipPrecision
            CLEARTYPE_QUALITY,               // Quality
            FIXED_PITCH | FF_ROMAN,      // PitchAndFamily
            _T("Segoe UI"));      // Facename

    BOLD_FONT.CreateFont(
        18,                            // Height
        0,                             // Width
        0,                             // Escapement
        0,                             // OrientationME
        FW_SEMIBOLD,                   // Weight
        FALSE,                         // Italic
        FALSE,                          // Underline
        0,                             // StrikeOut
        ANSI_CHARSET,                  // CharSet
        OUT_DEVICE_PRECIS,             // OutPrecision
        CLIP_DEFAULT_PRECIS,           // ClipPrecision
        CLEARTYPE_QUALITY,             // Quality
        FIXED_PITCH | FF_ROMAN,      // PitchAndFamily
        _T("Segoe UI"));      // Facename
    }
}