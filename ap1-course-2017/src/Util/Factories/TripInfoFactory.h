#ifndef AP1_TRIPINFOFACTORY_H
#define AP1_TRIPINFOFACTORY_H

#include "../../Core/Trip/TripInfo.h"

class TripInfoFactory {
public:
    TripInfoFactory();
    virtual ~TripInfoFactory();

    void setID(int id);
    void setStartTime(int time);
    void setStartX(int startX);
    void setStartY(int startY);

    void setEndX(int endX);
    void setEndY(int endY);

    void setNumOfPassengers(int numOfPassengers);
    void setTariff(float tariff);

    TripInfo* createTrip();

private:
    unsigned int _id;
    int start_x, start_y;
    int end_x, end_y;

    int _startTime;

    unsigned  int _passengers;
    float _tariff;
};


#endif //AP1_TRIPINFOFACTORY_H
