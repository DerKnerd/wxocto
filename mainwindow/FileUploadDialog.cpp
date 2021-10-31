//
// Created by imanuel on 30.10.21.
//

#include "FileUploadDialog.h"
#include "../octoprint/UploadFileThread.h"
#include "../octoprint/OctoApiEventIds.h"

FileUploadDialog::FileUploadDialog(wxWindow *parent)
        : wxDialog(parent, wxID_ANY, _("Upload file"), wxDefaultPosition, wxSize(-1, -1), wxDEFAULT_DIALOG_STYLE,
                   wxASCII_STR(wxDialogNameStr)) {
    auto dialogSizer = new wxBoxSizer(wxVERTICAL);
    auto itemsSizer = new wxFlexGridSizer(2);
    itemsSizer->SetFlexibleDirection(wxHORIZONTAL);
    itemsSizer->AddGrowableCol(1, 1);
    itemsSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    auto folderLabel = new wxStaticText(this, wxID_ANY, _("Folder"));
    auto fileLabel = new wxStaticText(this, wxID_ANY, _("File"));

    folder = new wxComboBox(this, wxID_ANY);
    file = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, _("Select a file"),
                                _("GCODE (*.gco;*.gcode)") + "|*.gco;*.gcode");

    itemsSizer->Add(folderLabel, 0, wxALL, FromDIP(5));
    itemsSizer->Add(folder, 0, wxALL, FromDIP(5));

    itemsSizer->Add(fileLabel, 0, wxALL, FromDIP(5));
    itemsSizer->Add(file, 0, wxALL | wxEXPAND, FromDIP(5));

    dialogSizer->Add(itemsSizer);

    auto dialogButtonSizer = CreateStdDialogButtonSizer(wxAPPLY | wxCLOSE);
    dialogSizer->Add(dialogButtonSizer, 1, wxGROW | wxALL, 5);
    this->SetSizerAndFit(dialogSizer);

    auto applyButton = dialogButtonSizer->GetApplyButton();
    applyButton->SetLabel(_("Upload file"));

    Bind(wxEVT_BUTTON, &FileUploadDialog::handleSave, this, wxID_APPLY);

    Bind(wxEVT_THREAD, &FileUploadDialog::handleFileUploaded, this, OctoApiEventId::OctoFileUploaded);
    Bind(wxEVT_THREAD, &FileUploadDialog::handleFileUploadError, this, OctoApiEventId::OctoFileUploadError);
}

void FileUploadDialog::setFolders(const std::vector<wxString> &folders) {
    for (const auto &item: folders) {
        folder->Append(item);
    }
}

void FileUploadDialog::handleSave(wxCommandEvent &event) {
    auto uploadFile = new UploadFileThread(this, folder->GetValue(), file->GetPath());
    uploadFile->Run();
}

void FileUploadDialog::handleFileUploaded(wxThreadEvent &event) {
    AcceptAndClose();
}

void FileUploadDialog::handleFileUploadError(wxThreadEvent &event) {
    wxMessageBox(_("Failed to upload file"));
}
