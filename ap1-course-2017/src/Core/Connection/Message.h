#ifndef AP1_MESSAGE_H
#define AP1_MESSAGE_H
#include <stdint.h>
#include <string>

struct Message{
    uint8_t op;
    std::string str;
};
#endif //AP1_MESSAGE_H
