//
// Created by imanuel on 10.10.21.
//

#ifndef WXOCTO_FETCHPRINTSTATUSTHREAD_H
#define WXOCTO_FETCHPRINTSTATUSTHREAD_H


#include <wx/thread.h>
#include <wx/window.h>

class FetchPrintStatusThread : public wxThread {
private:
    wxWindow *parent;
public:
    explicit FetchPrintStatusThread(wxWindow *parent);

protected:
    void *Entry() override;
};


#endif //WXOCTO_FETCHPRINTSTATUSTHREAD_H
