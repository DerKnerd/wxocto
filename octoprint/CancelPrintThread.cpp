//
// Created by imanuel on 10.10.21.
//

#include "CancelPrintThread.h"
#include "../MainApp.h"
#include "OctoApiEventIds.h"
#include "httpClient.h"

void *CancelPrintThread::Entry() {
    try {
        auto httpClient = getClient();
        auto response = httpClient.Post("/api/job", R"({"command": "cancel"})", "application/json");
        if (response.error() == httplib::Error::Success && response->status == 204) {
            auto event = new wxThreadEvent();
            event->SetId(OctoApiEventId::OctoPrintCancelled);
            wxQueueEvent(parent, event);
        } else {
            throw std::exception();
        }
    } catch (const std::exception &e) {
        auto event = new wxThreadEvent();
        event->SetPayload(e);
        event->SetId(OctoApiEventId::OctoPrintCancelError);
        wxQueueEvent(parent, event);
    }

    return nullptr;
}

CancelPrintThread::CancelPrintThread(wxWindow *parent) : wxThread(wxThreadKind::wxTHREAD_DETACHED), parent(parent) {}
