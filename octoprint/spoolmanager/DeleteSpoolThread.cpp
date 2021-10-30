//
// Created by imanuel on 23.10.21.
//

#include "DeleteSpoolThread.h"
#include "../OctoApiEventIds.h"
#include "../../MainApp.h"
#include "../httpClient.h"

void *DeleteSpoolThread::Entry() {
    auto response = getClient().Get(
            std::string("/plugin/SpoolManager/deleteSpool/" + std::to_string(spoolId)).c_str());
    auto event = new wxThreadEvent();
    if (response.error() == httplib::Error::Success && response->status == 200) {
        event->SetId(OctoApiEventId::OctoPrintSpoolManagerSpoolDeleted);
    } else {
        event->SetId(OctoApiEventId::OctoPrintSpoolManagerSpoolDeleteError);
    }
    wxQueueEvent(parent, event);

    return nullptr;
}

DeleteSpoolThread::DeleteSpoolThread(wxWindow *parent, long spoolId) : wxThread(wxThreadKind::wxTHREAD_DETACHED),
                                                                       parent(parent), spoolId(spoolId) {}
