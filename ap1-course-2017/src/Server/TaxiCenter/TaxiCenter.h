#ifndef AP1_EX1_TaxiCenter_H
#define AP1_EX1_TaxiCenter_H

#include "DriverDatabase.h"
#include "PathCalculation.h"
#include "../../Core/Trip/TripEndedListener.h"
#include "../../Core/Vehicles/Taxi.h"
#include "../../Core/Map.h"
#include "../../Core/Threading/ThreadPool.h"
#include "../../Core/Entities/Driver.h"

using namespace std;
class TaxiCenter : public TripEndedListener{
public:
	TaxiCenter(Map *map);
	virtual ~TaxiCenter();

	bool addDriver(Driver* d);
    Driver* getEmployee(int id);

	bool addTaxi(Taxi* t);
    bool assignTaxiTo(int taxiID, int driverID);

    bool addTrip(TripInfo *t);
    int assignTrip(int tripID);
    vector<int> assignAllTrips();
    void tripEnded(const int & tripID);

	void updateClock();

    void moveAll();

protected:
    struct UnassignedTrip{
        TripInfo* info;
        PathCalculation *path;
    };

    Map *_map;
    vector<Driver*> employees;
    DriverDatabase *driverDatabase;

    int _clock;

    vector<Taxi*> unassignedTaxis;

    ThreadPool* tp;
    vector<UnassignedTrip*> trips;
};

#endif 