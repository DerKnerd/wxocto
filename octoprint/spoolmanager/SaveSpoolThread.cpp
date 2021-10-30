//
// Created by imanuel on 23.10.21.
//

#include <utility>
#include "SaveSpoolThread.h"
#include "../../MainApp.h"
#include "../OctoApiEventIds.h"
#include "../httpClient.h"

void *SaveSpoolThread::Entry() {
    try {
        auto client = getClient();
        auto response = client.Post("/plugin/SpoolManager/saveSpool", spool.dump(), "application/json");
        if (response.error() == httplib::Error::Success && response->status == 200) {
            auto event = new wxThreadEvent();
            event->SetId(OctoApiEventId::OctoPrintSpoolManagerSpoolSaved);
            wxQueueEvent(parent, event);
        } else {
            throw std::exception();
        }
    } catch (const std::exception &e) {
        auto event = new wxThreadEvent();
        event->SetPayload(e);
        event->SetId(OctoApiEventId::OctoPrintSpoolManagerSpoolSaveError);
        wxQueueEvent(parent, event);
    }

    return nullptr;
}

SaveSpoolThread::SaveSpoolThread(wxWindow *parent, nlohmann::json spool) : parent(parent), spool(std::move(spool)) {}
