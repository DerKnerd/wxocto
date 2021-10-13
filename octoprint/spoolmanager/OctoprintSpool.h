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

    std::string displayName;
    std::string colorName;
    std::string color;
    std::string vendor;
    std::string material;
    double density;
    double diameter;
    double tolerance;

    double temperatureTool;
    double temperatureBed;
    std::string totalWeight;
    std::string usedWeight;
    std::string spoolWeight;
    std::string leftWeight;
    double totalLength;
    double usedLength;
    std::string leftLength;

    std::string purchasedFrom;
    double cost;
    std::string costUnit;
    wxDateTime purchasedOn;
    wxDateTime lastUsed;

    [[nodiscard]] std::string getPurchasedOn() const;
    [[nodiscard]] std::string getLastUsed() const;
    [[nodiscard]] std::string getUsedLength() const;
    [[nodiscard]] std::string getTotalLength() const;

    static OctoprintSpool fromJson(const nlohmann::json &json);
};


#endif //WXOCTO_OCTOPRINTSPOOL_H
