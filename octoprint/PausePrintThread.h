//
// Created by imanuel on 10.10.21.
//

#ifndef WXOCTO_PAUSEPRINTTHREAD_H
#define WXOCTO_PAUSEPRINTTHREAD_H

#include <wx/thread.h>
#include <wx/window.h>

class PausePrintThread : public wxThread {
private:
    wxWindow *parent;
public:
    explicit PausePrintThread(wxWindow *parent);

protected:
    void *Entry() override;
};


#endif //WXOCTO_PAUSEPRINTTHREAD_H
