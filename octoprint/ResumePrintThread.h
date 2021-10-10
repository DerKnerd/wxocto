//
// Created by imanuel on 10.10.21.
//

#ifndef WXOCTO_RESUMEPRINTTHREAD_H
#define WXOCTO_RESUMEPRINTTHREAD_H

#include <wx/thread.h>
#include <wx/window.h>

class ResumePrintThread : public wxThread {
private:
    wxWindow *parent;
public:
    explicit ResumePrintThread(wxWindow *parent);

protected:
    void *Entry() override;
};


#endif //WXOCTO_RESUMEPRINTTHREAD_H
