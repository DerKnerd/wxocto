//
// Created by imanuel on 17.10.21.
//

#ifndef WXOCTO_EDITSPOOLDIALOG_H
#define WXOCTO_EDITSPOOLDIALOG_H

#include "SpoolDialogBase.h"

class EditSpoolDialog : public SpoolDialogBase {
public:
    explicit EditSpoolDialog(wxWindow *parent);

protected:
    void handleSave(wxCommandEvent &event) override;
};


#endif //WXOCTO_EDITSPOOLDIALOG_H
