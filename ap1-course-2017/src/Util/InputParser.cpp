#include "InputParser.h"
#include "Factories/DriverFactory.h"
#include "Factories/TaxiFactory.h"
#include "Factories/TripInfoFactory.h"

/**
 * Inputs a driver, constructs a DriverInput struct, to hold thd vehicle ID.
 * @return pointer to DriverInput contatining the driver and the vehicle ID.
 * If bad input received, it returns NULL ptr;
 */
DriverInput* InputParser::inputDriver() {
    int tempInt;
    short tempShort;
    char tempChar;
    DriverFactory driverFactory;
    DriverInput* din = NULL;
    //if wrong input is inputted, exception will be thrown from
    //the taxiFactory. catch it.
    try {
        //input driver's ID
        cin >> tempInt >> tempChar; //get id then comma
        driverFactory.setID(tempInt);

        //input driver's age
        cin >> tempShort >> tempChar; //input age then comma
        driverFactory.setAge(tempShort);

        //input driver's marital status
        cin >> tempChar; //input marital status char
        driverFactory.setMaritalStatus(tempChar);

        //input driver's experience years
        cin >> tempChar >> tempShort >> tempChar; //input comma then experience years, then another comma
        driverFactory.setExperienceYears(tempShort);

        //input the driver's vehicle id
        cin >> tempInt; //input vehicle id.
        //prevent negative vehicle id
        if (tempInt < 0)
            return NULL;
        din = new DriverInput;
        din->vehicleID = tempInt;
        din->driver = driverFactory.createDriver();
    }catch(exception){}
    //if bad input received, then din is null, as expected.
    return din;
}

/**
 * Inputs a taxi.
 * @return pointer to Taxi object constructed from the received input.
* If bad input received, function returns NULL ptr;
 */
Taxi *InputParser::inputTaxi() {
    int tempInt;
    short tempShort;
    char tempChar;
    TaxiFactory taxiFactory;
    Taxi* t = NULL;
    //if wrong input is inputted, exception will be thrown from
    //the taxiFactory. catch it.
    try {
        //input taxi's ID
        cin >> tempInt >> tempChar; //input ID then input comma
        taxiFactory.setID(tempInt);

        //input the taxi's type
        cin >> tempShort >> tempChar; //input taxi type, then input comma
        taxiFactory.setType(tempShort);

        //input taxi's manufacturer
        cin >> tempChar; //input manufacturer char
        taxiFactory.setManufacturer(tempChar);

        //input taxi's color
        cin >> tempChar >> tempChar; //input comma then input the color char
        taxiFactory.setColor(tempChar);

        t = taxiFactory.createTaxi();
    }catch(exception){}

    //if bad input received, then t is null, as expected.
    return t;
}

/**
 * Inputs a tripInfo.
 * @return pointer to TripInfo object constructed from the received input.
* If bad input received, function returns NULL ptr;
 */
TripInfo *InputParser::inputTripInfo() {
    int tempInt;
    float tempFloat;
    char tempChar;
    TripInfoFactory tripInfoFactory;
    TripInfo *t = NULL;

    //if wrong input is inputted, exception will be thrown from
    //the taxiFactory. catch it.
    try {
        //input trip's ID
        cin >> tempInt >> tempChar; //input ID then input comma.
        tripInfoFactory.setID(tempInt);

        //input trip's start point
        cin >> tempInt >> tempChar; //input start X and then input comma
        tripInfoFactory.setStartX(tempInt);
        cin >> tempInt >> tempChar; //input startY and then input comma
        tripInfoFactory.setStartY(tempInt);

        //input trip's end point
        cin >> tempInt >> tempChar; //input end X and then input comma
        tripInfoFactory.setEndX(tempInt);
        cin >> tempInt >> tempChar; //input end Y and then input comma
        tripInfoFactory.setEndY(tempInt);

        //input trip's number of passengers.
        cin >> tempInt >> tempChar; //input num of passengers, then input comma
        tripInfoFactory.setNumOfPassengers(tempInt);
        cin >> tempFloat >> tempChar; //input tariff, then input comma
        tripInfoFactory.setTariff(tempFloat);
        cin >> tempInt; //input startTime.
        tripInfoFactory.setStartTime(tempInt);

        t = tripInfoFactory.createTrip();
    }catch(exception) {}
    //if bad input received, then t is null, as expected.
    return t;
}
