//
// Created by imanuel on 08.10.21.
//

#ifndef WXOCTO_OCTOPRINTFILE_H
#define WXOCTO_OCTOPRINTFILE_H

#include <string>
#include <nlohmann/json.hpp>
#include <wx/datetime.h>

class OctoprintFile {
public:
    enum FileType {
        Folder,
        File,
    };

public:
    wxString name;
    wxString path;
    long size = 0L;
    time_t uploaded = 0;
    double depth = 0.0;
    double height = 0.0;
    double width = 0.0;
    double filamentLength = 0.0;
    double estimatedPrintTime = 0.0;
    std::vector<OctoprintFile> children;
    FileType type;
    wxString origin;

    [[nodiscard]] wxString getDimensions() const;

    [[nodiscard]] wxString getPrintTime() const;
    [[nodiscard]] wxString getSize() const;

    static OctoprintFile fromJson(const nlohmann::json &json);
};


#endif //WXOCTO_OCTOPRINTFILE_H
