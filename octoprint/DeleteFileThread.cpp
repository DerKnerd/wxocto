//
// Created by imanuel on 29.10.21.
//

#include "DeleteFileThread.h"
#include "OctoApiEventIds.h"
#include "../MainApp.h"

#include <utility>
#include <easyhttpcpp/EasyHttp.h>

void *DeleteFileThread::Entry() {
    auto settings = MainApp::getInstance()->GetSettings();
    auto request = easyhttpcpp::Request::Builder()
            .httpDelete()
            .setUrl((settings.server + "/api/files/" + file.origin + "/" + file.path).utf8_string())
            .setHeader("X-Api-Key", settings.apiKey)
            .build();
    try {
        auto httpClient = easyhttpcpp::EasyHttp::Builder().build();
        auto call = httpClient->newCall(request);
        auto response = call->execute();
        if (response->isSuccessful() && response->getCode() == 204) {
            auto event = new wxThreadEvent();
            event->SetId(OctoApiEventId::OctoFileDeleted);
            wxQueueEvent(parent, event);
        }
    } catch (const std::exception &e) {
        auto event = new wxThreadEvent();
        event->SetPayload(e);
        event->SetId(OctoApiEventId::OctoFileDeleteError);
        wxQueueEvent(parent, event);
    }

    return nullptr;
}

DeleteFileThread::DeleteFileThread(wxWindow *parent, OctoprintFile file) :
        wxThread(wxThreadKind::wxTHREAD_DETACHED),
        parent(parent),
        file(std::move(file)) {}
