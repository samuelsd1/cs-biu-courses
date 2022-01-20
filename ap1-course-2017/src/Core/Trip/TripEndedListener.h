#ifndef AP1_TRIPENDEDLISTENER_H
#define AP1_TRIPENDEDLISTENER_H

/**
 * Trip Ended Listener interface.
 */
class TripEndedListener{
public:
    virtual void tripEnded(const int & tripID) = 0;
};

#endif //AP1_TRIPENDEDLISTENER_H
