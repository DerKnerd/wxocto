//
// Created by imanuel on 10.10.21.
//

#include <easyhttpcpp/EasyHttp.h>

#include "SelectAndPrintFileThread.h"
#include "OctoApiEventIds.h"
#include "../MainApp.h"

wxThread::ExitCode SelectAndPrintFileThread::Entry() {
    auto settings = MainApp::getInstance()->GetSettings();
    auto body = easyhttpcpp::RequestBody::create(
            easyhttpcpp::MediaType::Ptr(new easyhttpcpp::MediaType("application/json")),
            Poco::makeShared<std::string>(R"({"print": true, "command": "select"})"));
    auto request = easyhttpcpp::Request::Builder()
            .httpPost(body)
            .setUrl(settings.server + "/api/files/" + origin.utf8_string() + "/" + filepath.utf8_string())
            .setHeader("X-Api-Key", settings.apiKey)
            .build();
    try {
        auto httpClient = easyhttpcpp::EasyHttp::Builder().build();
        auto call = httpClient->newCall(request);
        auto response = call->execute();
        if (response->isSuccessful() && response->getCode() == 204) {
            auto event = new wxThreadEvent();
            event->SetId(OctoApiEventId::OctoPrintStarted);
            wxQueueEvent(parent, event);
        } else {
            auto responseBody = response->getBody()->toString();
            throw std::exception();
        }
    } catch (const std::exception &e) {
        auto event = new wxThreadEvent();
        event->SetPayload(e);
        event->SetId(OctoApiEventId::OctoPrintStartError);
        wxQueueEvent(parent, event);
    }

    return nullptr;
}

SelectAndPrintFileThread::SelectAndPrintFileThread(wxWindow *parent, wxString origin, wxString filepath)
        : wxThread(wxThreadKind::wxTHREAD_DETACHED),
          parent(parent),
          origin(origin),
          filepath(filepath) {}

