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
    explicit SelectAndPrintFileThread(wxWindow *parent, std::string origin, std::string filepath);

private:
    wxWindow *parent;
    std::string filepath;
    std::string origin;

protected:
    void *Entry() override;
};


#endif //WXOCTO_SELECTANDPRINTFILETHREAD_H
