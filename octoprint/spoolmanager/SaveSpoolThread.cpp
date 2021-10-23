//
// Created by imanuel on 23.10.21.
//

#include <easyhttpcpp/EasyHttp.h>

#include <utility>
#include "SaveSpoolThread.h"
#include "../../MainApp.h"
#include "../OctoApiEventIds.h"

void *SaveSpoolThread::Entry() {
    auto settings = MainApp::getInstance()->GetSettings();
    auto body = easyhttpcpp::RequestBody::create(
            easyhttpcpp::MediaType::Ptr(new easyhttpcpp::MediaType("application/json")),
            Poco::makeShared<std::string>(spool.dump()));
    auto purchasedOn = spool.dump();
    auto request = easyhttpcpp::Request::Builder()
            .httpPut(body)
            .setUrl(settings.server + "/plugin/SpoolManager/saveSpool")
            .setHeader("X-Api-Key", settings.apiKey)
            .build();

    try {
        auto httpClient = easyhttpcpp::EasyHttp::Builder().build();
        auto call = httpClient->newCall(request);
        auto response = call->execute();
        if (response->isSuccessful() && response->getCode() == 200) {
            auto event = new wxThreadEvent();
            auto errorData = response->getBody()->toString();
            event->SetId(OctoApiEventId::OctoPrintSpoolManagerSaved);
            wxQueueEvent(parent, event);
        } else {
            throw std::exception();
        }
    } catch (const std::exception &e) {
        auto event = new wxThreadEvent();
        event->SetPayload(e);
        event->SetId(OctoApiEventId::OctoPrintSpoolManagerSaveError);
        wxQueueEvent(parent, event);
    }

    return nullptr;
}

SaveSpoolThread::SaveSpoolThread(wxWindow *parent, nlohmann::json spool) : parent(parent), spool(std::move(spool)) {}
