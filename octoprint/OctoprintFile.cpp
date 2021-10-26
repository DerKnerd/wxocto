//
// Created by imanuel on 08.10.21.
//

#include <sstream>
#include <iomanip>
#include "OctoprintFile.h"

OctoprintFile OctoprintFile::fromJson(const nlohmann::json &json) {
    auto file = OctoprintFile();
    if (json.contains("date")) {
        file.uploaded = json["date"];
    } else {
        file.uploaded = 0;
    }
    file.path = wxString::FromUTF8(json["path"]);
    file.name = wxString::FromUTF8(json["display"]);
    file.size = json["size"];
    file.origin = wxString::FromUTF8(json["origin"]);
    if (json.contains("gcodeAnalysis")) {
        auto gcodeAnalysis = json["gcodeAnalysis"];
        if (gcodeAnalysis.contains("estimatedPrintTime")) {
            file.estimatedPrintTime = gcodeAnalysis["estimatedPrintTime"];
        }
        if (gcodeAnalysis.contains("filament") && gcodeAnalysis["filament"].contains("tool0") &&
            gcodeAnalysis["filament"]["tool0"].contains("length")) {
            file.filamentLength = gcodeAnalysis["filament"]["tool0"]["length"];
        }
        if (gcodeAnalysis.contains("dimensions")) {
            auto dimensions = gcodeAnalysis["dimensions"];
            file.depth = dimensions["depth"];
            file.height = dimensions["height"];
            file.width = dimensions["width"];
        }
    }
    file.type = FileType::File;
    if (json["type"] == "folder") {
        file.type = FileType::Folder;
    }

    return file;
}

wxString OctoprintFile::getDimensions() const {
    if (width == 0 && height == 0 && depth == 0) {
        return "";
    }

    auto ss = std::wstringstream();
    ss << std::fixed << std::setprecision(2) << width << L"mm \u00D7 " << height << L"mm \u00D7 " << depth << L"mm";

    return ss.str();
}

wxString OctoprintFile::getPrintTime() const {
    if (estimatedPrintTime == 0) {
        return "";
    }

    auto time = estimatedPrintTime / 60.0 / 60.0;
    auto ss = std::stringstream();
    ss << std::fixed << std::setprecision(2) << time << "h";

    return ss.str();
}

wxString OctoprintFile::getSize() const {
    if (size == 0) {
        return "";
    }

    auto mb = (double) size / 1024.0 / 1024.0;
    auto ss = std::stringstream();
    ss << std::fixed << std::setprecision(2) << mb << "MB";

    return ss.str();
}

