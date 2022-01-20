#ifndef AP1_SERIALIZATION_H
#define AP1_SERIALIZATION_H

#include <string>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class Serialization {
public:
    template<typename T>
    static std::string serialize(T *object) {
        if (object == NULL)
            throw "Serialization::serialize: NULL object received.";
        //serialize the object
        std::string serial_str;
        boost::iostreams::back_insert_device<std::string> inserter(serial_str);
        boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
        boost::archive::binary_oarchive oa(s);
        oa << object;
        s.flush();

        return serial_str;
    }

    /**
     * function deserializes an object from the received string.
     * if it failed to serialize the object, it returns NULL
     * @tparam T - the type of the object to deserialize into
     * @param serial_str - the serialized string
     * @return pointer to the deserialized object.
     */
    template<typename T>
    static T *deserialize(const std::string & serial_str) {
        T *obj = NULL;
        //try to deserialize the object from the serial_str if failed return null.
        try {
            boost::iostreams::basic_array_source<char> device(serial_str.c_str(), serial_str.length());
            boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
            boost::archive::binary_iarchive ia(s);
            ia >> obj;
            return obj;
        } catch (std::exception& e) {
            return NULL;
        }
    }
};


#endif //AP1_SERIALIZATION_H