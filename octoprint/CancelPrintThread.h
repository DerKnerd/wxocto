//
// Created by imanuel on 10.10.21.
//

#ifndef WXOCTO_CANCELPRINTTHREAD_H
#define WXOCTO_CANCELPRINTTHREAD_H

#include <wx/thread.h>
#include <wx/window.h>

class CancelPrintThread : public wxThread {
private:
    wxWindow *parent;
public:
    explicit CancelPrintThread(wxWindow *parent);

protected:
    void *Entry() override;
};


#endif //WXOCTO_CANCELPRINTTHREAD_H
