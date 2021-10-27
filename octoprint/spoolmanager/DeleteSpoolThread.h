//
// Created by imanuel on 23.10.21.
//

#ifndef WXOCTO_DELETESPOOLTHREAD_H
#define WXOCTO_DELETESPOOLTHREAD_H

#include <wx/thread.h>
#include <wx/window.h>

class DeleteSpoolThread : public wxThread {
private:
    wxWindow *parent;
    long spoolId = -1L;

public:
    explicit DeleteSpoolThread(wxWindow *parent, long spoolId);

protected:
    void *Entry() override;
};


#endif //WXOCTO_DELETESPOOLTHREAD_H
