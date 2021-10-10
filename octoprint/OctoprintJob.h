//
// Created by imanuel on 09.10.21.
//

#ifndef WXOCTO_OCTOPRINTJOB_H
#define WXOCTO_OCTOPRINTJOB_H

#include <chrono>
#include <string>
#include <nlohmann/json.hpp>

class OctoprintJob {
public:
    double printProgress;
    time_t timeElapsed;
    time_t timeLeft;
    std::string file;

    [[nodiscard]]std::string getTimeElapsed() const;

    [[nodiscard]]std::string getTimeLeft() const;

    [[nodiscard]]std::string getFinishTime() const;

    static OctoprintJob fromJson(const nlohmann::json &json);
};


#endif //WXOCTO_OCTOPRINTJOB_H
