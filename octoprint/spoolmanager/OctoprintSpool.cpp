//
// Created by imanuel on 11.10.21.
//

#include <iomanip>
#include "OctoprintSpool.h"

std::string OctoprintSpool::getPurchasedOn() const {
    return purchasedOn.Format().ToStdString();
}

OctoprintSpool OctoprintSpool::fromJson(const nlohmann::json &json) {
    auto spool = OctoprintSpool();
    if (json["bedTemperature"].is_number()) {
        spool.temperatureBed = json["bedTemperature"];
    }
    if (json["databaseId"].is_number()) {
        spool.databaseId = json["databaseId"];
    }
    if (json["displayName"].is_string()) {
        spool.displayName = json["displayName"];
    }
    if (json["colorName"].is_string()) {
        spool.colorName = json["colorName"];
    }
    if (json["color"].is_string()) {
        spool.color = json["color"];
    }
    if (json["vendor"].is_string()) {
        spool.vendor = json["vendor"];
    }
    if (json["material"].is_string()) {
        spool.material = json["material"];
    }
    if (json["density"].is_number()) {
        spool.density = json["density"];
    }
    if (json["diameter"].is_number()) {
        spool.diameter = json["diameter"];
    }
    if (json["diameterTolerance"].is_number()) {
        spool.tolerance = json["diameterTolerance"];
    }
    if (json["temperature"].is_number()) {
        spool.temperatureTool = json["temperature"];
    }
    if (json["totalWeight"].is_string()) {
        spool.totalWeight = json["totalWeight"];
    }
    if (json["usedWeight"].is_string()) {
        spool.usedWeight = json["usedWeight"];
    }
    if (json["spoolWeight"].is_string()) {
        spool.spoolWeight = json["spoolWeight"];
    }
    if (json["remainingWeight"].is_string()) {
        spool.leftWeight = json["remainingWeight"];
    }
    if (json["totalLength"].is_number()) {
        spool.totalLength = json["totalLength"];
    }
    if (json["usedLength"].is_number()) {
        spool.usedLength = json["usedLength"];
    }
    if (json["remainingLength"].is_string()) {
        spool.leftLength = json["remainingLength"];
    }
    if (json["purchasedFrom"].is_string()) {
        spool.purchasedFrom = json["purchasedFrom"];
    }
    if (json["purchasedOn"].is_string()) {
        std::string purchasedOn = json["purchasedOn"];
        spool.purchasedOn = wxDateTime();
        spool.purchasedOn.ParseDate(purchasedOn);
    }
    if (json["lastUse"].is_string()) {
        std::string lastUsed = json["lastUse"];
        spool.lastUsed = wxDateTime();
        spool.lastUsed.ParseDate(lastUsed);
    }

    return spool;
}

std::string OctoprintSpool::getLastUsed() const {
    if (!lastUsed.IsValid()) {
        return "";
    }
    return lastUsed.Format().ToStdString();
}

std::string OctoprintSpool::getUsedLength() const {
    auto ss = std::stringstream();
    ss << std::fixed << std::setprecision(2) << usedLength;

    return ss.str();
}

std::string OctoprintSpool::getTotalLength() const {
    auto ss = std::stringstream();
    ss << std::fixed << std::setprecision(2) << totalLength;

    return ss.str();
}
