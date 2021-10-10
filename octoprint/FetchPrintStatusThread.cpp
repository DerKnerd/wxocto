//
// Created by imanuel on 10.10.21.
//

#include <easyhttpcpp/EasyHttp.h>
#include "FetchPrintStatusThread.h"
#include "../MainApp.h"
#include "OctoprintJob.h"
#include "OctoApiEventIds.h"

wxThread::ExitCode FetchPrintStatusThread::Entry() {
    auto settings = MainApp::getInstance()->GetSettings();
    auto request = easyhttpcpp::Request::Builder()
            .httpGet()
            .setUrl(settings.server + "/api/job")
            .setHeader("X-Api-Key", settings.apiKey)
            .build();
    try {
        auto httpClient = easyhttpcpp::EasyHttp::Builder().build();
        auto call = httpClient->newCall(request);
        auto response = call->execute();
        if (response->isSuccessful() && response->getCode() == 200) {
            auto body = response->getBody()->toString();
            auto jsonBody = nlohmann::json::parse(body);

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
