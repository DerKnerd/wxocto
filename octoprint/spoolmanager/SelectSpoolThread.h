//
// Created by imanuel on 28.10.21.
//

#ifndef WXOCTO_SELECTSPOOLTHREAD_H
#define WXOCTO_SELECTSPOOLTHREAD_H

#include <wx/thread.h>
#include <wx/window.h>

class SelectSpoolThread : public wxThread {
private:
    wxWindow *parent;
    long spoolId;

public:
    explicit SelectSpoolThread(wxWindow *parent, long spoolId);

protected:
    void *Entry() override;
};


#endif // WXOCTO_SELECTSPOOLTHREAD_H
