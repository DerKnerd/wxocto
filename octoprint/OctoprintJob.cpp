//
// Created by imanuel on 09.10.21.
//

#include <sstream>
#include <wx/datetime.h>
#include <wx/translation.h>
#include <iomanip>
#include "OctoprintJob.h"

wxString OctoprintJob::getTimeElapsed() const {
    if (state != Printing && state != Paused && state != Pausing) {
        return _("No print started").ToStdString();
    }

    auto ss = std::stringstream();
    auto minutes = timeElapsed / 60;
    auto hours = minutes / 60;

    ss << hours << ":" << std::setfill('0') << std::setw(2) << int(minutes % 60) << ":" << std::setfill('0')
       << std::setw(2) << int(timeElapsed % 60);

    return ss.str();
}

wxString OctoprintJob::getTimeLeft() const {
    if (timeLeft == 0) {
        return _("No print started").ToStdString();
    }

    auto ss = std::stringstream();
    auto minutes = timeLeft / 60;
    auto hours = minutes / 60;

    ss << hours << ":" << std::setfill('0') << std::setw(2) << int(minutes % 60) << ":" << std::setfill('0')
       << std::setw(2) << int(timeLeft % 60);

    return ss.str();
}

wxString OctoprintJob::getFinishTime() const {
    if (timeLeft == 0) {
        return _("No print started");
    }

    auto seconds = timeLeft % 60;
    auto minutes = (timeLeft / 60) % 60;
    auto hours = (timeLeft / 60 / 60) % 60;

    auto date = wxDateTime::Now();
    date += wxTimeSpan(hours, minutes, seconds);

    return date.Format().ToStdString();
}

OctoprintJob OctoprintJob::fromJson(const nlohmann::json &json) {
    auto job = OctoprintJob();
    if (json.contains("job")) {
        auto jsonJob = json["job"];
        if (jsonJob["file"].is_null()) {
            job.file = _("No print started");
            job.fileSelected = false;
            job.path = "";
            job.origin = "";
        } else {
            auto jsonFile = jsonJob["file"];
            if (jsonFile["name"].is_null()) {
                job.file = _("No file selected");
                job.fileSelected = false;
            } else {
                job.file = wxString::FromUTF8(jsonFile["name"]);
                job.fileSelected = true;
            }
            if (jsonFile["path"].is_null()) {
                job.path = "";
            } else {
                job.path = wxString::FromUTF8(jsonFile["path"]);
            }
            if (jsonFile["origin"].is_null()) {
                job.origin = "";
            } else {
                job.origin = wxString::FromUTF8(jsonFile["origin"]);
            }
        }
    } else {
        job.file = _("No print started");
        job.fileSelected = false;
    }

    if (json.contains("progress")) {
        auto progress = json["progress"];
        if (progress["completion"].is_null()) {
            job.printProgress = 0;
        } else {
            job.printProgress = progress["completion"];
        }
        if (progress["printTimeLeft"].is_null()) {
            job.timeLeft = 0;
        } else {
            job.timeLeft = progress["printTimeLeft"];
        }
        if (progress["printTime"].is_null()) {
            job.timeElapsed = 0;
        } else {
            job.timeElapsed = progress["printTime"];
        }
    } else {
        job.timeLeft = 0;
        job.timeElapsed = 0;
        job.printProgress = 0;
    }

    auto state = wxString::FromUTF8(json["state"].get<std::string>());
    if (state == "Operational") {
        job.state = OctoprintJobState::Operational;
    } else if (state == "Printing") {
        job.state = OctoprintJobState::Printing;
    } else if (state == "Pausing") {
        job.state = OctoprintJobState::Pausing;
    } else if (state == "Paused") {
        job.state = OctoprintJobState::Paused;
    } else if (state == "Cancelling") {
        job.state = OctoprintJobState::Cancelling;
    } else if (state == "Offline") {
        job.state = OctoprintJobState::Offline;
    }

    return job;
}
