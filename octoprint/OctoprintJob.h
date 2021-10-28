//
// Created by imanuel on 09.10.21.
//

#ifndef WXOCTO_OCTOPRINTJOB_H
#define WXOCTO_OCTOPRINTJOB_H

#include <chrono>
#include <string>
#include <nlohmann/json.hpp>
#include "OctoprintFile.h"

enum OctoprintJobState {
    Operational,
    Printing,
    Pausing,
    Paused,
    Cancelling,
    Error,
    Offline
};

class OctoprintJob {
public:
    double printProgress;
    time_t timeElapsed;
    time_t timeLeft;
    wxString file;
    OctoprintJobState state;
    wxString path;
    wxString origin;
    bool fileSelected;
    double filamentLength;

    [[nodiscard]]wxString getTimeElapsed() const;

    [[nodiscard]]wxString getTimeLeft() const;

    [[nodiscard]]wxString getFinishTime() const;

    static OctoprintJob fromJson(const nlohmann::json &json);
};


#endif //WXOCTO_OCTOPRINTJOB_H
