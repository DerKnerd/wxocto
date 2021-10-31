//
// Created by imanuel on 30.10.21.
//

#include <fstream>
#include <sstream>
#include <filesystem>
#include <httplib/httplib.h>
#include "UploadFileThread.h"
#include "httpClient.h"
#include "OctoApiEventIds.h"
#include <wx/event.h>

UploadFileThread::UploadFileThread(wxWindow *parent, const wxString &parentFolder,
                                   const wxString &localFilePath)
        : wxThread(), parent(parent), parentFolder(parentFolder), localFilePath(localFilePath) {
}

void *UploadFileThread::Entry() {
    auto file = std::ifstream();
    file.open(localFilePath.utf8_string());
    auto content = std::string();
    file >> content;

    auto os = std::stringstream();
    auto filename = std::filesystem::path(localFilePath.utf8_string()).filename();
    httplib::MultipartFormDataItems data = {
            {"path", parentFolder.utf8_string()},
            {"file", content, filename.string(), "application/octet-stream"},
    };

    auto result = getClient().Post("/api/files/local", data);
    auto event = new wxThreadEvent();
    if (result->status == 201) {
        event->SetId(OctoApiEventId::OctoFileUploaded);
    } else {
        event->SetId(OctoApiEventId::OctoFileUploadError);
    }

    wxQueueEvent(parent, event);

    return nullptr;
}
