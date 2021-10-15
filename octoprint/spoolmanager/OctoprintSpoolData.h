//
// Created by imanuel on 15.10.21.
//

#ifndef WXOCTO_OCTOPRINTSPOOLDATA_H
#define WXOCTO_OCTOPRINTSPOOLDATA_H

#include <vector>
#include "OctoprintSpool.h"

class OctoprintSpoolData {
public:
    std::vector<OctoprintSpool*> spools;
    int selectedSpool;
};


#endif //WXOCTO_OCTOPRINTSPOOLDATA_H
