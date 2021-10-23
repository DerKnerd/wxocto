//
// Created by imanuel on 23.10.21.
//

#ifndef WXOCTO_SAVESPOOLTHREAD_H
#define WXOCTO_SAVESPOOLTHREAD_H

#include <wx/thread.h>
#include <wx/window.h>
#include "OctoprintSpool.h"

class SaveSpoolThread : public wxThread {
private:
    wxWindow *parent;
    nlohmann::json spool;
public:
    explicit SaveSpoolThread(wxWindow *parent, nlohmann::json spool);
protected:
    void *Entry() override;
};


#endif //WXOCTO_SAVESPOOLTHREAD_H
