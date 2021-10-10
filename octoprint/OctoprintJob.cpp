//
// Created by imanuel on 09.10.21.
//

#include <sstream>
#include <wx/datetime.h>
#include <wx/translation.h>
#include <iomanip>
#include "OctoprintJob.h"

std::string OctoprintJob::getTimeElapsed() const {
    if (timeElapsed == 0) {
        return _("No print started").ToStdString();
    }

    auto ss = std::stringstream();
    auto minutes = timeElapsed / 60;
    auto hours = minutes / 60;

    ss << hours << ":" << std::setfill('0') << std::setw(2) << int(minutes % 60) << ":" << std::setfill('0')
       << std::setw(2) << int(timeElapsed % 60);

    return ss.str();
}

std::string OctoprintJob::getTimeLeft() const {
    if (timeLeft == 0 && file.empty()) {
        return _("No print started").ToStdString();
    } else if (timeLeft == 0) {
        return _("Print finished").ToStdString();
    }

    auto ss = std::stringstream();
    auto minutes = timeLeft / 60;
    auto hours = minutes / 60;

    ss << hours << ":" << std::setfill('0') << std::setw(2) << int(minutes % 60) << ":" << std::setfill('0')
       << std::setw(2) << int(timeLeft % 60);

    return ss.str();
}

std::string OctoprintJob::getFinishTime() const {
    if (timeLeft == 0 && file.empty()) {
        return _("No print started").ToStdString();
    } else if (timeLeft == 0) {
        return _("Print finished").ToStdString();
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
        if (jsonJob["file"]["name"].is_null()) {
            job.file = _("No print started");
        } else {
            job.file = jsonJob["file"]["name"];
        }
    } else {
        job.file = _("No print started");
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

    return job;
}
