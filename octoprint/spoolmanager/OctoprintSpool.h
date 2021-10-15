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
    long databaseId;

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
    wxString leftLength;

    wxString purchasedFrom;
    double cost;
    wxString costUnit;
    wxDateTime purchasedOn;
    wxDateTime lastUsed;

    [[nodiscard]] wxString getPurchasedOn() const;

    [[nodiscard]] wxString getLastUsed() const;

    [[nodiscard]] wxString getUsedLength() const;

    [[nodiscard]] wxString getTotalLength() const;

    static OctoprintSpool *fromJson(const nlohmann::json &json);
};


#endif //WXOCTO_OCTOPRINTSPOOL_H
