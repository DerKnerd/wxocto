//
// Created by imanuel on 30.10.21.
//

#ifndef WXOCTO_FILEUPLOADDIALOG_H
#define WXOCTO_FILEUPLOADDIALOG_H

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include <wx/filepicker.h>

class FileUploadDialog : public wxDialog {
public:
    explicit FileUploadDialog(wxWindow *parent);

    void setFolders(const std::vector<wxString> &folders);

private:
    wxComboBox *folder;
    wxFilePickerCtrl *file;

    void handleSave(wxCommandEvent &event);

    void handleFileUploaded(wxThreadEvent &event);

    void handleFileUploadError(wxThreadEvent &event);
};

#endif //WXOCTO_FILEUPLOADDIALOG_H
