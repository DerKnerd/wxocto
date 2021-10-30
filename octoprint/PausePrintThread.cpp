//
// Created by imanuel on 10.10.21.
//

#include "PausePrintThread.h"
#include "../MainApp.h"
#include "OctoApiEventIds.h"
#include "httpClient.h"

void *PausePrintThread::Entry() {
    auto event = new wxThreadEvent();
    auto response = getClient().Post("/api/job", R"({"command": "pause", "action": "pause"})", "application/json");
    if (response.error() == httplib::Error::Success && response->status == 204) {
        event->SetId(OctoApiEventId::OctoPrintPaused);
    } else {
        event->SetId(OctoApiEventId::OctoPrintPauseError);
    }

    wxQueueEvent(parent, event);

    return nullptr;
}

PausePrintThread::PausePrintThread(wxWindow *parent) : wxThread(wxThreadKind::wxTHREAD_DETACHED), parent(parent) {}
