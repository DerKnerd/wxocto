//
// Created by imanuel on 09.10.21.
//

#include <easyhttpcpp/EasyHttp.h>
#include "FetchFilesThread.h"
#include "../MainApp.h"
#include "OctoprintFile.h"

FetchFilesThread::FetchFilesThread(wxWindow *parent) : wxThread(wxThreadKind::wxTHREAD_DETACHED), parent(parent) {}

wxThread::ExitCode FetchFilesThread::Entry() {
    auto settings = MainApp::getInstance()->GetSettings();
    auto request = easyhttpcpp::Request::Builder()
            .httpGet()
            .setUrl(settings.server + "/api/files?recursive=true")
            .setHeader("X-Api-Key", settings.apiKey)
            .build();
    try {
        auto httpClient = easyhttpcpp::EasyHttp::Builder().build();
        auto call = httpClient->newCall(request);
        auto response = call->execute();
        if (response->isSuccessful() && response->getCode() == 200) {
            auto body = response->getBody()->toString();
            auto jsonBody = nlohmann::json::parse(body);
            auto files = std::vector<OctoprintFile>();
            for (const auto &item: jsonBody["files"]) {
                files.push_back(handleFile(item));
            }

            auto event = new wxThreadEvent();
            event->SetPayload(files);
            event->SetId(FetchFilesThread::OctoFilesFetched);
            wxQueueEvent(parent, event);
        } else {
            throw std::exception();
        }
    } catch (const std::exception &e) {
        auto event = new wxThreadEvent();
        event->SetPayload(e);
        event->SetId(FetchFilesThread::OctoFilesError);
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
