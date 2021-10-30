//
// Created by imanuel on 10.10.21.
//

#include "FetchPrintStatusThread.h"
#include "../MainApp.h"
#include "OctoprintJob.h"
#include "OctoApiEventIds.h"
#include "httpClient.h"

wxThread::ExitCode FetchPrintStatusThread::Entry() {
    try {
        auto response = getClient().Get("/api/job");
        if (response.error() == httplib::Error::Success && response->status == 200) {
            auto jsonBody = nlohmann::json::parse(response->body);

            auto event = new wxThreadEvent();
            event->SetPayload(OctoprintJob::fromJson(jsonBody));
            event->SetId(OctoApiEventId::OctoJobFetched);
            wxQueueEvent(parent, event);
        } else {
            throw std::exception();
        }
    } catch (const std::exception &e) {
        auto event = new wxThreadEvent();
        event->SetPayload(e);
        event->SetId(OctoApiEventId::OctoJobError);
        wxQueueEvent(parent, event);
    }

    return nullptr;
}

FetchPrintStatusThread::FetchPrintStatusThread(wxWindow *parent) : wxThread(wxThreadKind::wxTHREAD_DETACHED),
                                                                   parent(parent) {}
