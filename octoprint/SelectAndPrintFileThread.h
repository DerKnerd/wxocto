//
// Created by imanuel on 10.10.21.
//

#ifndef WXOCTO_SELECTANDPRINTFILETHREAD_H
#define WXOCTO_SELECTANDPRINTFILETHREAD_H


#include <wx/thread.h>
#include <wx/window.h>

class SelectAndPrintFileThread : public wxThread {
private:
public:
    explicit SelectAndPrintFileThread(wxWindow *parent, wxString origin, wxString filepath);

private:
    wxWindow *parent;
    wxString filepath;
    wxString origin;

protected:
    void *Entry() override;
};


#endif //WXOCTO_SELECTANDPRINTFILETHREAD_H
