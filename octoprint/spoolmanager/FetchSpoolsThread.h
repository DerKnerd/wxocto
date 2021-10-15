//
// Created by imanuel on 11.10.21.
//

#ifndef WXOCTO_FETCHSPOOLSTHREAD_H
#define WXOCTO_FETCHSPOOLSTHREAD_H

#include <wx/thread.h>
#include <wx/window.h>

class FetchSpoolsThread : public wxThread {
private:
    wxWindow *parent;

public:
    explicit FetchSpoolsThread(wxWindow *parent);

protected:
    void *Entry() override;
};


#endif //WXOCTO_FETCHSPOOLSTHREAD_H
