//
// Created by imanuel on 11.10.21.
//

#include <nlohmann/json.hpp>
#include <easyhttpcpp/EasyHttp.h>
#include "FetchSpoolsThread.h"
#include "OctoprintSpool.h"
#include "../../MainApp.h"
#include "../OctoApiEventIds.h"
#include "OctoprintSpoolData.h"

void *FetchSpoolsThread::Entry() {
    auto settings = MainApp::getInstance()->GetSettings();
    auto request = easyhttpcpp::Request::Builder()
            .httpGet()
            .setUrl(settings.server +
                    "/plugin/SpoolManager/loadSpoolsByQuery?from=0&to=9999999&sortColumn=displayName&sortOrder=asc&filterName=all")
            .setHeader("X-Api-Key", settings.apiKey)
            .build();
    try {
        auto httpClient = easyhttpcpp::EasyHttp::Builder().build();
        auto call = httpClient->newCall(request);
        auto response = call->execute();
        if (response->isSuccessful() && response->getCode() == 200) {
            auto body = response->getBody()->toString();
            auto jsonBody = nlohmann::json::parse(body);
            auto spoolData = OctoprintSpoolData();
            auto selectedSpoolId = -1;
            for (const auto &item: jsonBody["selectedSpools"]) {
                if (!item.is_null()) {
                    selectedSpoolId = item["databaseId"];
                    break;
                }
            }

            spoolData.selectedSpool = selectedSpoolId;
            spoolData.materials = std::vector<wxString>();
            spoolData.vendors = std::vector<wxString>();
            spoolData.spools = std::vector<OctoprintSpool *>();

            auto spools = jsonBody["allSpools"].get<std::vector<nlohmann::json>>();
            auto materials = jsonBody["catalogs"]["materials"].get<std::vector<nlohmann::json>>();
            auto vendors = jsonBody["catalogs"]["vendors"].get<std::vector<nlohmann::json>>();

            for (const auto &item: spools) {
                spoolData.spools.emplace_back(OctoprintSpool::fromJson(item));
            }
            for (const auto &item: materials) {
                spoolData.materials.emplace_back(wxString::FromUTF8(item));
            }
            for (const auto &item: vendors) {
                spoolData.vendors.emplace_back(wxString::FromUTF8(item));
            }

            auto event = new wxThreadEvent();
            event->SetPayload(spoolData);
            event->SetId(OctoApiEventId::OctoPrintSpoolManagerSpoolsFetched);
            wxQueueEvent(parent, event);
        } else {
            throw std::exception();
        }
    } catch (const std::exception &e) {
        auto event = new wxThreadEvent();
        event->SetPayload(e);
        event->SetId(OctoApiEventId::OctoPrintSpoolManagerSpoolsFetchError);
        wxQueueEvent(parent, event);
    }

    return nullptr;
}

FetchSpoolsThread::FetchSpoolsThread(wxWindow *parent) : wxThread(wxThreadKind::wxTHREAD_DETACHED), parent(parent) {}
