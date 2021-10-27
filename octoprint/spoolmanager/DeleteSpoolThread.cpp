//
// Created by imanuel on 23.10.21.
//

#include <easyhttpcpp/EasyHttp.h>
#include "DeleteSpoolThread.h"
#include "../OctoApiEventIds.h"
#include "../../MainApp.h"

void *DeleteSpoolThread::Entry() {
    auto settings = MainApp::getInstance()->GetSettings();
    auto request = easyhttpcpp::Request::Builder()
            .httpDelete()
            .setUrl(settings.server +
                    "/plugin/SpoolManager/deleteSpool/" + std::to_string(spoolId))
            .setHeader("X-Api-Key", settings.apiKey)
            .build();
    try {
        auto httpClient = easyhttpcpp::EasyHttp::Builder().build();
        auto call = httpClient->newCall(request);
        auto response = call->execute();
        if (response->isSuccessful() && response->getCode() == 200) {
            auto event = new wxThreadEvent();
            event->SetId(OctoApiEventId::OctoPrintSpoolManagerSpoolDeleted);
            wxQueueEvent(parent, event);
        }
    } catch (const std::exception &e) {
        auto event = new wxThreadEvent();
        event->SetPayload(e);
        event->SetId(OctoApiEventId::OctoPrintSpoolManagerSpoolDeleteError);
        wxQueueEvent(parent, event);
    }

    return nullptr;
}

DeleteSpoolThread::DeleteSpoolThread(wxWindow *parent, long spoolId) : wxThread(wxThreadKind::wxTHREAD_DETACHED),
                                                                      parent(parent), spoolId(spoolId) {}
