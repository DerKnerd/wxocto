//
// Created by imanuel on 10.10.21.
//

#include "SelectAndPrintFileThread.h"
#include "OctoApiEventIds.h"
#include "../MainApp.h"
#include "httpClient.h"

wxThread::ExitCode SelectAndPrintFileThread::Entry() {
    auto event = new wxThreadEvent();
    auto response = getClient().Post(("/api/files/" + origin + "/" + filepath).c_str(),
                                     R"({"print": true, "command": "select"})", "application/json");
    if (response.error() == httplib::Error::Success && response->status == 204) {
        event->SetId(OctoApiEventId::OctoPrintStarted);
    } else {
        event->SetId(OctoApiEventId::OctoPrintStartError);
    }

    wxQueueEvent(parent, event);

    return nullptr;
}

SelectAndPrintFileThread::SelectAndPrintFileThread(wxWindow *parent, const wxString &origin, const wxString &filepath)
        : wxThread(wxThreadKind::wxTHREAD_DETACHED),
          parent(parent),
          origin(origin),
          filepath(filepath) {}

