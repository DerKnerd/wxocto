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
    file.name = json["display"];
    file.size = json["size"];
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

std::string OctoprintFile::getDimensions() const {
    if (width == 0 && height == 0 && depth == 0) {
        return "";
    }

    auto ss = std::stringstream();
    ss << std::fixed << std::setprecision(2) << width << "mm × " << height << "mm × " << depth << "mm";

    return ss.str();
}

std::string OctoprintFile::getPrintTime() const {
    if (estimatedPrintTime == 0) {
        return "";
    }

    auto time = estimatedPrintTime / 60.0 / 60.0;
    auto ss = std::stringstream();
    ss << std::fixed << std::setprecision(2) << time << "h";

    return ss.str();
}

std::string OctoprintFile::getSize() const {
    if (size == 0) {
        return "";
    }

    auto mb = (double) size / 1024.0 / 1024.0;
    auto ss = std::stringstream();
    ss << std::fixed << std::setprecision(2) << mb << "MB";

    return ss.str();
}

