#include "TripEndedNotifier.h"

/**
 * adds a tripEndedListener to the notifier
 * @param listener
 * @return bool signifies if the listener was added to the notifier
 */
bool TripEndedNotifier::addTripEndedListener(TripEndedListener *listener) {
    for (unsigned int i = 0; i < listeners.size(); i++) {
        //if the tripEndedListener already exists, return false
        if (listeners[i] == listener) {
            return false;
        }
    }
    //if didnt found it , means we can add it.
    listeners.push_back(listener); //add the listener
    return true;
}

/**
 * removes a tripEndedListener from the notifier
 * @param listener
 * @return bool signifies if the listener was removed from the notifier
 */
bool TripEndedNotifier::removeTripEndedListener(const TripEndedListener *listener) {
    for (unsigned int i = 0; i < listeners.size(); i++) {
        //if found the tripEndedListener, delete it and return true
        if (listeners[i] == listener) {
            listeners.erase(listeners.begin() + i);
            return true;
        }
    }
    return false;
}

/**
 * notifies all the tripListeners that a trip with the given ID has ended.
 * @param tripID - the tripID of the ended trip.
 */
void TripEndedNotifier::notifyAll(int tripID) {
    std::vector<TripEndedListener *> listenersCopy = listeners;
    for (unsigned int i = 0; i < listenersCopy.size(); i++) {
        listenersCopy[i]->tripEnded(tripID);
    }
}