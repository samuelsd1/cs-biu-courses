#ifndef AP1_INPUTPARSER_H
#define AP1_INPUTPARSER_H


#include "../Core/Entities/Driver.h"
#include "DriverInput.h"
#include "../Core/Vehicles/Taxi.h"

class InputParser {
public:
    static DriverInput* inputDriver();

    static Taxi* inputTaxi();

    static TripInfo* inputTripInfo();
};


#endif //AP1_INPUTPARSER_H
