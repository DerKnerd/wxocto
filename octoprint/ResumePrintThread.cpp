//
// Created by imanuel on 10.10.21.
//

#include "ResumePrintThread.h"
#include "../MainApp.h"
#include "OctoApiEventIds.h"
#include "httpClient.h"

void *ResumePrintThread::Entry() {
    auto event = new wxThreadEvent();
    auto response = getClient().Post("/api/job", R"({"command": "pause", "action": "resume"})", "application/json");
    if (response.error() == httplib::Error::Success && response->status == 204) {
        event->SetId(OctoApiEventId::OctoPrintResumed);
    } else {
        event->SetId(OctoApiEventId::OctoPrintResumeError);
    }

    wxQueueEvent(parent, event);

    return nullptr;
}

ResumePrintThread::ResumePrintThread(wxWindow *parent) : wxThread(wxThreadKind::wxTHREAD_DETACHED), parent(parent) {}
