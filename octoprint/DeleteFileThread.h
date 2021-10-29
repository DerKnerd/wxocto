//
// Created by imanuel on 29.10.21.
//

#ifndef WXOCTO_DELETEFILETHREAD_H
#define WXOCTO_DELETEFILETHREAD_H

#include <wx/thread.h>
#include <wx/window.h>
#include "OctoprintFile.h"

class DeleteFileThread : public wxThread {
private:
    wxWindow *parent;
    OctoprintFile file;

public:
    DeleteFileThread( wxWindow *parent, OctoprintFile file);

    void *Entry() override;
};


#endif //WXOCTO_DELETEFILETHREAD_H
