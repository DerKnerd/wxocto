//
// Created by imanuel on 17.10.21.
//

#include "AddSpoolDialog.h"
#include "../octoprint/OctoApiEventIds.h"
#include "../octoprint/spoolmanager/SaveSpoolThread.h"

void AddSpoolDialog::handleSave(wxCommandEvent &event) {
    auto spool = getSpool();
    auto data = spool.toJson();
    auto thread = new SaveSpoolThread(this, data);
    thread->Run();
}

AddSpoolDialog::AddSpoolDialog(wxWindow *parent) : SpoolDialogBase(parent, _("Add spool"),
                                                                   SpoolDialogBase::AddSpoolDialog) {
    Bind(wxEVT_THREAD, &AddSpoolDialog::handleSpoolSaved, this, OctoApiEventId::OctoPrintSpoolManagerSpoolSaved);
    Bind(wxEVT_THREAD, &AddSpoolDialog::handleSpoolSaveError, this, OctoApiEventId::OctoPrintSpoolManagerSpoolSaveError);
}

void AddSpoolDialog::handleSpoolSaved(wxThreadEvent &event) {
    AcceptAndClose();
}

void AddSpoolDialog::handleSpoolSaveError(wxThreadEvent &event) {
    wxMessageBox(_("Failed to save the spool"), wxString::FromAscii(wxMessageBoxCaptionStr), wxICON_ERROR | wxCENTRE);
}
