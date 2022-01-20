#ifndef AP1_CALCULATETRIPTASK_H
#define AP1_CALCULATETRIPTASK_H

#include <stdlib.h>
#include "../../Core/Threading/Task.h"
#include "../TaxiCenter/PathCalculation.h"
#include "../../Core/Trip/TripInfo.h"
#include "../../Util/easylogging++.h"

class CalculatePathTask : public Task {
public:
    CalculatePathTask(const Map *map, const TripInfo *tripInfo, PathCalculation *p);

    virtual ~CalculatePathTask();
    void run();
    void showTask() {}

private:
    const Map *_map;
    const TripInfo *_tripInfo;
    PathCalculation *_p;
};


#endif //AP1_CALCULATETRIPTASK_H
