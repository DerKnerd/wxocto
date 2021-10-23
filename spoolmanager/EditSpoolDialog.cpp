//
// Created by imanuel on 17.10.21.
//

#include "EditSpoolDialog.h"
#include "../octoprint/spoolmanager/SaveSpoolThread.h"
#include "../octoprint/OctoApiEventIds.h"

void EditSpoolDialog::handleSave(wxCommandEvent &event) {
    auto spool = getSpool();
    auto data = spool.toJson();
    data["databaseId"] = spool.databaseId;
    auto thread = new SaveSpoolThread(this, data);
    thread->Run();
}

EditSpoolDialog::EditSpoolDialog(wxWindow *parent) : SpoolDialogBase(parent, _("Edit spool"),
                                                                     SpoolDialogBase::EditSpoolDialog) {
    Bind(wxEVT_THREAD, &EditSpoolDialog::handleSpoolSaved, this, OctoApiEventId::OctoPrintSpoolManagerSaved);
    Bind(wxEVT_THREAD, &EditSpoolDialog::handleSpoolSaveError, this, OctoApiEventId::OctoPrintSpoolManagerSaveError);
}

void EditSpoolDialog::handleSpoolSaved(wxThreadEvent &event) {
    AcceptAndClose();
}

void EditSpoolDialog::handleSpoolSaveError(wxThreadEvent &event) {
    wxMessageBox(_("Failed to save the spool"), wxString::FromAscii(wxMessageBoxCaptionStr), wxICON_ERROR | wxCENTRE);
}
