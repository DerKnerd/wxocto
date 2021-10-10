//
// Created by imanuel on 09.10.21.
//

#ifndef WXOCTO_FETCHFILESTHREAD_H
#define WXOCTO_FETCHFILESTHREAD_H

#include <wx/event.h>
#include <wx/thread.h>
#include "OctoprintFile.h"

class FetchFilesThread : public wxThread {
private:
    wxWindow *parent;

    OctoprintFile handleFile(const nlohmann::json &json);

public:
    explicit FetchFilesThread(wxWindow *parent);

protected:
    void *Entry() override;
};


#endif //WXOCTO_FETCHFILESTHREAD_H
