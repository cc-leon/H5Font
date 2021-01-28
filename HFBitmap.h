#pragma once

class HFBitmap : public CBitmap {
public:
    HFBitmap();
    virtual ~HFBitmap();

    BOOL LoadFromFile(CString CONST& sBmpFilename);
    BOOL SaveToFile(CString CONST& sBmpFilename);
    CSize GetBmpCSize();
};

