//
// Created by imanuel on 29.10.21.
//

#include "DeleteFileThread.h"
#include "OctoApiEventIds.h"
#include "../MainApp.h"
#include "httpClient.h"

#include <utility>

void *DeleteFileThread::Entry() {
    auto event = new wxThreadEvent();
    auto httpClient = getClient();
    auto response = httpClient.Delete(("/api/files/" + file.origin + "/" + file.path).c_str());
    if (response.error() == httplib::Error::Success && response->status == 204) {
        event->SetId(OctoApiEventId::OctoFileDeleted);
    } else {
        event->SetId(OctoApiEventId::OctoFileDeleteError);
    }
    wxQueueEvent(parent, event);

    return nullptr;
}

DeleteFileThread::DeleteFileThread(wxWindow *parent, OctoprintFile file) :
        wxThread(wxThreadKind::wxTHREAD_DETACHED),
        parent(parent),
        file(std::move(file)) {}
