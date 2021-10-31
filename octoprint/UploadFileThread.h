//
// Created by imanuel on 30.10.21.
//

#ifndef WXOCTO_UPLOADFILETHREAD_H
#define WXOCTO_UPLOADFILETHREAD_H


#include <wx/thread.h>
#include <wx/window.h>

class UploadFileThread : public wxThread {
private:
    wxWindow *parent;
    wxString parentFolder;
    wxString localFilePath;

public:
protected:
    void *Entry() override;

public:
    explicit UploadFileThread(wxWindow *parent, const wxString& parentFolder, const wxString& localFilePath);
};


#endif //WXOCTO_UPLOADFILETHREAD_H
