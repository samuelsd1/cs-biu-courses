#ifndef AP1_TRIPENDEDNOTIFIER_H
#define AP1_TRIPENDEDNOTIFIER_H


#include <vector>
#include "TripEndedListener.h"
#include "../../Util/easylogging++.h"

class TripEndedNotifier {
public:
    bool addTripEndedListener(TripEndedListener *listener);

    bool removeTripEndedListener(const TripEndedListener *listener);

protected:
    void notifyAll(int tripID);

private:
    std::vector<TripEndedListener *> listeners;
};


#endif //AP1_TRIPENDEDNOTIFIER_H
