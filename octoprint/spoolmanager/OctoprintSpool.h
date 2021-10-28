//
// Created by imanuel on 11.10.21.
//

#ifndef WXOCTO_OCTOPRINTSPOOL_H
#define WXOCTO_OCTOPRINTSPOOL_H


#include <string>
#include <wx/datetime.h>
#include <nlohmann/json.hpp>

class OctoprintSpool {
public:
    long databaseId = -1;
    long version = 1;

    wxString displayName;
    wxString colorName;
    wxString color;
    wxString vendor;
    wxString material;
    double density;
    double diameter;
    double tolerance;

    double temperatureTool;
    double temperatureBed;
    wxString totalWeight;
    wxString usedWeight;
    wxString spoolWeight;
    wxString leftWeight;
    double totalLength;
    double usedLength;
    double leftLength;

    wxString purchasedFrom;
    double cost;
    wxDateTime purchasedOn;
    wxDateTime lastUsed;
    wxDateTime firstUsed;

    [[nodiscard]] wxString getPurchasedOn() const;

    [[nodiscard]] wxString getLastUsed() const;

    [[nodiscard]] wxString getUsedLength() const;

    [[nodiscard]] wxString getTotalLength() const;

    [[nodiscard]] wxString getLeftLength() const;

    static OctoprintSpool *fromJson(const nlohmann::json &json);

    nlohmann::json toJson();
};


#endif //WXOCTO_OCTOPRINTSPOOL_H
