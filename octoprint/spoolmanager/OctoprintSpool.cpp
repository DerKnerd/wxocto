//
// Created by imanuel on 11.10.21.
//

#include <iomanip>
#include "OctoprintSpool.h"

wxString OctoprintSpool::getPurchasedOn() const {
    return purchasedOn.Format();
}

OctoprintSpool *OctoprintSpool::fromJson(const nlohmann::json &json) {
    auto spool = new OctoprintSpool();
    if (json["bedTemperature"].is_number()) {
        spool->temperatureBed = json["bedTemperature"];
    }
    if (json["databaseId"].is_number()) {
        spool->databaseId = json["databaseId"];
    }
    if (json["displayName"].is_string()) {
        spool->displayName = wxString::FromUTF8(json["displayName"]);
    }
    if (json["colorName"].is_string()) {
        spool->colorName = wxString::FromUTF8(json["colorName"]);
    }
    if (json["color"].is_string()) {
        spool->color = wxString::FromUTF8(json["color"]);
    }
    if (json["vendor"].is_string()) {
        spool->vendor = wxString::FromUTF8(json["vendor"]);
    }
    if (json["material"].is_string()) {
        spool->material = wxString::FromUTF8(json["material"]);
    }
    if (json["density"].is_number()) {
        spool->density = json["density"];
    }
    if (json["diameter"].is_number()) {
        spool->diameter = json["diameter"];
    }
    if (json["diameterTolerance"].is_number()) {
        spool->tolerance = json["diameterTolerance"];
    }
    if (json["temperature"].is_number()) {
        spool->temperatureTool = json["temperature"];
    }
    if (json["totalWeight"].is_string()) {
        spool->totalWeight = wxString::FromUTF8(json["totalWeight"]);
    }
    if (json["usedWeight"].is_string()) {
        spool->usedWeight = wxString::FromUTF8(json["usedWeight"]);
    }
    if (json["spoolWeight"].is_string()) {
        spool->spoolWeight = wxString::FromUTF8(json["spoolWeight"]);
    }
    if (json["remainingWeight"].is_string()) {
        spool->leftWeight = wxString::FromUTF8(json["remainingWeight"]);
    }
    if (json["totalLength"].is_number()) {
        spool->totalLength = json["totalLength"];
    }
    if (json["usedLength"].is_number()) {
        spool->usedLength = json["usedLength"];
    }
    if (json["remainingLength"].is_string()) {
        spool->leftLength = wxString::FromUTF8(json["remainingLength"]);
    }
    if (json["purchasedFrom"].is_string()) {
        spool->purchasedFrom = wxString::FromUTF8(json["purchasedFrom"]);
    }
    if (json["purchasedOn"].is_string()) {
        std::string purchasedOn = json["purchasedOn"];
        spool->purchasedOn = wxDateTime();
        spool->purchasedOn.ParseDate(purchasedOn);
    }
    if (json["lastUse"].is_string()) {
        std::string lastUsed = json["lastUse"];
        spool->lastUsed = wxDateTime();
        spool->lastUsed.ParseDate(lastUsed);
    }

    return spool;
}

wxString OctoprintSpool::getLastUsed() const {
    if (!lastUsed.IsValid()) {
        return "";
    }
    return lastUsed.Format().ToStdString();
}

wxString OctoprintSpool::getUsedLength() const {
    auto ss = std::stringstream();
    ss << std::fixed << std::setprecision(2) << usedLength;

    return ss.str();
}

wxString OctoprintSpool::getTotalLength() const {
    auto ss = std::stringstream();
    ss << std::fixed << std::setprecision(2) << totalLength;

    return ss.str();
}
