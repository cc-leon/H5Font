#pragma once

class HFRichTextBox : public CRichEditCtrl {
    DECLARE_DYNAMIC(HFRichTextBox)
public:
    HFRichTextBox();
    virtual ~HFRichTextBox();

protected:
    DECLARE_MESSAGE_MAP()
};
