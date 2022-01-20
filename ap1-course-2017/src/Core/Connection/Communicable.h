#ifndef AP1_COMMUNICABLE_H
#define AP1_COMMUNICABLE_H


#include "Message.h"
#include "Command.h"

#include <sys/socket.h>
#include <boost/lexical_cast.hpp>

#include "../../Util/easylogging++.h"
#include "../../Util/Serialization/Serialization.h"

#define COMM_BUFF_SIZE  65536
#define OP_END_CONN (0x00)
#define OP_MESSAGE  (0x01)
#define OP_OBJECT   (0x02)
#define OP_COMMAND  (0x03)

/**
 * Communicable class represents something you can communicate with using the socket.
 */
class Communicable {

public:
    Communicable(int descriptor);
    virtual ~Communicable();

    bool sendCloseSignal();

    bool sendMessage(Message* msg);
    bool sendString(std::string msg);
    bool sendCommand(Command c);

    template<typename T>
    bool sendObject(T *obj) {
        Message* m = buildMessage(obj);
        //send the object
        bool res = sendMessage(m);
        delete m;
        return res;
    }

    Message *receiveMessage();

    static Message* buildMessage(std::string msg);
    static Message* buildMessage(Command c);
    template <typename T>
    static Message* buildMessage(T *obj){
        if (obj == NULL)
            return NULL;

        Message* m = new Message();
        m->op= OP_OBJECT;
        //serialize the object
        m->str = Serialization::serialize(obj);

        return m;
    }
    static Message* closeSignal();
protected:
    int _descriptor;

    bool sendMessage(std::string msg, uint8_t op);

    bool isOpCode(uint8_t op) const;


};


#endif //AP1_COMMUNICABLE_H
