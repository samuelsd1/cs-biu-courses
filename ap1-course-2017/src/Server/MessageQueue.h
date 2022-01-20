#ifndef AP1_MESSAGEQUEUE_H
#define AP1_MESSAGEQUEUE_H


#include "../Core/SynchronizedQueue.h"
#include "../Core/Connection/Message.h"

class MessageQueue : public SynchronizedQueue<Message> {};


#endif //AP1_MESSAGEQUEUE_H
