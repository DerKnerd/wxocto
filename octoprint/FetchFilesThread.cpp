//
// Created by imanuel on 09.10.21.
//

#include "FetchFilesThread.h"
#include "../MainApp.h"
#include "OctoprintFile.h"
#include "OctoApiEventIds.h"
#include "httpClient.h"

FetchFilesThread::FetchFilesThread(wxWindow *parent) : wxThread(wxThreadKind::wxTHREAD_DETACHED), parent(parent) {}

wxThread::ExitCode FetchFilesThread::Entry() {
    try {
        auto client = getClient();
        auto result = client.Get("/api/files?recursive=true");
        if (result.error() == httplib::Error::Success && result->status == 200) {
            auto jsonBody = nlohmann::json::parse(result->body);
            auto files = std::vector<OctoprintFile>();
            for (const auto &item: jsonBody["files"]) {
                files.push_back(handleFile(item));
            }

            auto event = new wxThreadEvent();
            event->SetPayload(files);
            event->SetId(OctoApiEventId::OctoFilesFetched);
            wxQueueEvent(parent, event);
        } else {
            throw std::exception();
        }
    } catch (const std::exception &e) {
        auto event = new wxThreadEvent();
        event->SetPayload(e);
        event->SetId(OctoApiEventId::OctoFilesError);
        wxQueueEvent(parent, event);
    }

    return nullptr;
}

OctoprintFile FetchFilesThread::handleFile(const nlohmann::json &json) {
    auto file = OctoprintFile::fromJson(json);
    if (json["type"] == "folder") {
        for (const auto &item: json["children"]) {
            file.children.push_back(handleFile(item));
        }
    }

    return file;
}
