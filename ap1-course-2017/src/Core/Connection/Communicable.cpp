#include "Communicable.h"

/**
 * Communicable Ctor
 * @param socket - the socket to use for communicating with the communicable object.
 */
Communicable::Communicable(int descriptor) : _descriptor(descriptor) {}

/**
 * Communicable Dtor
 */
Communicable::~Communicable() {}

/**
 * sends end connection signal.
 */
bool Communicable::sendCloseSignal() {
    return sendMessage(std::string(), OP_END_CONN);
}

/**
 * Sends a message through the socket
 * @param msg - the message to send
 * @param op - the op code of the message. can be one of the following:
 *              * OP_END_CONN 0x00 - connection closed
 *              * OP_MESSAGE  0x01 - data is a string message
 *              * OP_OBJECT   0x02 - data is a serialized object
 *              * OP_COMMAND  0x03 - data is a command.
 * @return true if the message was sent successfully, false otherwise.
 */
bool Communicable::sendMessage(std::string msg, uint8_t op) {
    if (!isOpCode(op)) {
        LERROR << "Communicable::sendMessage() illegal op code received, not sending...";
        return false;
    }

    msg.insert(msg.begin(), op); //insert the op at the beginning of the string.
    size_t len = msg.size();
    if (len >= COMM_BUFF_SIZE) {
        LERROR << "Communicable::sendMessage() message exceeds the buffer size, not sending...";
        return false;
    }
    const char *buff = msg.c_str();
    LDEBUG << "Trying to send message to of size " << msg.size() << " to descriptor " << _descriptor << ".";
    try {
        ssize_t bytes = send(_descriptor, buff, len, 0);
        if (bytes >= 0) {
            LDEBUG << "message was sent successfully";
            return true;
        }
    } catch (...) {
        LERROR << "could not send the message";
    }
    return false;
}

/**
 * Sends a string message over the socket
 * @param msg - the string to be sent
 * @return bool signifies if the message was sent successfully.
 */
bool Communicable::sendString(std::string msg) {
    return sendMessage(msg, OP_MESSAGE);
}

/**
 * function sends a command over the socket
 */
bool Communicable::sendCommand(Command c) {
    return sendMessage(boost::lexical_cast<std::string>(c), OP_COMMAND);
}

/**
 * function receives data from the serial port,
 * and returns Message object containing opcode and the data received.
 * if a problem occured, NULL is returned.
 * @return
 */
Message *Communicable::receiveMessage() {
    char buff[COMM_BUFF_SIZE];
    LDEBUG << "waiting to receive message from descriptor " << _descriptor << ".";
    ssize_t readBytes = recv(_descriptor, buff, sizeof(buff), 0);
    if (readBytes < 0) {
        LERROR << "Communicable: couldn't read message from descriptor " << _descriptor << ".";
        return NULL;
    }

    //save the inputted data in the string
    std::string receivedMessage = std::string(buff, (size_t) readBytes);
    LDEBUG << "Received message with size " << receivedMessage.size() << " from descriptor " << _descriptor << ".";
    //put the string in a message object.
    Message *m = new Message; //create a new message to return.
    m->op = (uint8_t) receivedMessage[0]; //set the op code
    receivedMessage.erase(receivedMessage.begin()); //delete the op code from the string
    m->str = receivedMessage; //set the string in the Message
    return m;
}

/**
 * function gets an op code, and returns true
 * if the op code is one of the Communicable op codes.
 */
bool Communicable::isOpCode(uint8_t op) const {
    switch (op) {
        case OP_END_CONN:
            return true;
        case OP_MESSAGE:
            return true;
        case OP_OBJECT:
            return true;
        case OP_COMMAND:
            return true;
        default:
            return false;
    }
}

bool Communicable::sendMessage(Message *msg) {
    if (msg == NULL) {
        LERROR << "Communicable::sendMessage: NULL Message received";
        return false;
    }
    return sendMessage(msg->str, msg->op);
}

Message *Communicable::buildMessage(std::string msg) {
    Message *m = new Message();
    m->op = OP_MESSAGE;
    m->str = msg;
    return m;
}

Message *Communicable::buildMessage(Command c) {
    Message *m = new Message();
    m->str = boost::lexical_cast<std::string>(c);
    m->op = OP_COMMAND;
    return m;
}

Message *Communicable::closeSignal() {
    Message *m = new Message();
    m->str = std::string();
    m->op = OP_END_CONN;
    return m;
}
