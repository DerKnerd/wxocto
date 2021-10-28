//
// Created by imanuel on 28.10.21.
//

#include <easyhttpcpp/EasyHttp.h>
#include "SelectSpoolThread.h"
#include "../OctoApiEventIds.h"
#include "../../MainApp.h"

SelectSpoolThread::SelectSpoolThread(wxWindow *parent, long spoolId) : wxThread(wxThreadKind::wxTHREAD_DETACHED),
                                                                       spoolId(spoolId), parent(parent) {}

void *SelectSpoolThread::Entry() {
    auto settings = MainApp::getInstance()->GetSettings();
    auto body = easyhttpcpp::RequestBody::create(
            easyhttpcpp::MediaType::Ptr(new easyhttpcpp::MediaType("application/json")),
            Poco::makeShared<std::string>(nlohmann::json{
                    {"databaseId", spoolId},
                    {"toolIndex", 0}
            }.dump()));
    auto request = easyhttpcpp::Request::Builder()
            .httpPut(body)
            .setUrl(settings.server + "/plugin/SpoolManager/selectSpool")
            .setHeader("X-Api-Key", settings.apiKey)
            .build();

    try {
        auto httpClient = easyhttpcpp::EasyHttp::Builder().build();
        auto call = httpClient->newCall(request);
        auto response = call->execute();
        if (response->isSuccessful() && response->getCode() == 200) {
            auto event = new wxThreadEvent();
            auto errorData = response->getBody()->toString();
            event->SetId(OctoApiEventId::OctoPrintSpoolManagerSpoolSelected);
            wxQueueEvent(parent, event);
        } else {
            throw std::exception();
        }
    } catch (const std::exception &e) {
        auto event = new wxThreadEvent();
        event->SetPayload(e);
        event->SetId(OctoApiEventId::OctoPrintSpoolManagerSpoolSelectError);
        wxQueueEvent(parent, event);
    }

    return nullptr;
}
