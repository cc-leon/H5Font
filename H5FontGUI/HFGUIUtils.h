#pragma once
//
// Functions used to help manipulation on GUI
//
namespace ui {
    // Get the CRect of a control in its parent window
    // hWnd is the handle to the control of interest
    CRect GetWindowCRectInParent(CWnd * pWnd);

    // Given a rectangle, its new size and the pivot point (inside the CRect), 
    // calculate the new CRect region it should position
    CRect ZoomCRectOverCPoint(CSize CONST& csNew, CRect CONST& rcOld, CPoint CONST& ptPivot);

    // Checking if rect1 is inside rect2
    // When rect1's either dimension (horizontal or vertical) is less or equal than that of rect2, it must stay inside;
    // Else, rect2 must stay in rect1 on the same dimension
    BOOL CRect1InsideCRect2(CRect CONST& rect1, CRect CONST& rect2);

};
