//
// Created by imanuel on 10.10.21.
//

#include <easyhttpcpp/Request.h>
#include <easyhttpcpp/EasyHttp.h>
#include "PausePrintThread.h"
#include "../MainApp.h"
#include "OctoApiEventIds.h"

void *PausePrintThread::Entry() {
    auto settings = MainApp::getInstance()->GetSettings();
    auto body = easyhttpcpp::RequestBody::create(
            easyhttpcpp::MediaType::Ptr(new easyhttpcpp::MediaType("application/json")),
            Poco::makeShared<std::string>(R"({"command": "pause", "action": "pause"})"));
    auto request = easyhttpcpp::Request::Builder()
            .httpPost(body)
            .setUrl(settings.server + "/api/job")
            .setHeader("X-Api-Key", settings.apiKey)
            .build();

    try {
        auto httpClient = easyhttpcpp::EasyHttp::Builder().build();
        auto call = httpClient->newCall(request);
        auto response = call->execute();
        if (response->isSuccessful() && response->getCode() == 204) {
            auto event = new wxThreadEvent();
            event->SetId(OctoApiEventId::OctoPrintPaused);
            wxQueueEvent(parent, event);
        } else {
            auto responseBody = response->getBody()->toString();
            throw std::exception();
        }
    } catch (const std::exception &e) {
        auto event = new wxThreadEvent();
        event->SetPayload(e);
        event->SetId(OctoApiEventId::OctoPrintPauseError);
        wxQueueEvent(parent, event);
    }

    return nullptr;
}

PausePrintThread::PausePrintThread(wxWindow *parent) : wxThread(wxThreadKind::wxTHREAD_DETACHED), parent(parent) {}