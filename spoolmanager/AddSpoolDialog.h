//
// Created by imanuel on 17.10.21.
//

#ifndef WXOCTO_ADDSPOOLDIALOG_H
#define WXOCTO_ADDSPOOLDIALOG_H

#include "SpoolDialogBase.h"

class AddSpoolDialog : public SpoolDialogBase {
protected:
public:
    explicit AddSpoolDialog(wxWindow *parent);

protected:
    void handleSave(wxCommandEvent &event) override;
};


#endif //WXOCTO_ADDSPOOLDIALOG_H
