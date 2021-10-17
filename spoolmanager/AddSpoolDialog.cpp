//
// Created by imanuel on 17.10.21.
//

#include "AddSpoolDialog.h"

void AddSpoolDialog::handleSave(wxCommandEvent &event) {
    wxMessageBox("Henlo, please commit die!");
}

AddSpoolDialog::AddSpoolDialog(wxWindow *parent) : SpoolDialogBase(parent, _("Add spool")) {}
