//
// Created by imanuel on 28.10.21.
//

#include "SelectSpoolThread.h"
#include "../OctoApiEventIds.h"
#include "../../MainApp.h"
#include "../httpClient.h"

SelectSpoolThread::SelectSpoolThread(wxWindow *parent, long spoolId) : wxThread(wxThreadKind::wxTHREAD_DETACHED),
                                                                       spoolId(spoolId), parent(parent) {}

void *SelectSpoolThread::Entry() {
    try {
        auto httpClient = getClient();
        auto response = httpClient.Put("/plugin/SpoolManager/selectSpool", nlohmann::json{
                {"databaseId", spoolId},
                {"toolIndex",  0}
        }.dump(), "application/json");
        if (response.error() == httplib::Error::Success && response->status == 200) {
            auto event = new wxThreadEvent();
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
