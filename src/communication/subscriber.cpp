#include "communication/subscriber.h"


Subscriber::Subscriber(CommsDataType dataType, std::string portId) : type(dataType), socket(ctx, zmq::socket_type::sub) {
    int confl = 1;
    socket.setsockopt(ZMQ_CONFLATE, &confl, sizeof(int));
    socket.connect(portId);
    socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    port = portId;
    for(size_t i = 0; i < typeLengths[type]; i++)
        values.push_back(0.0);
}

Subscriber::Subscriber(const Subscriber& subscriber) : type(subscriber.type), socket(ctx, zmq::socket_type::sub){
    int confl = 1;
    socket.setsockopt(ZMQ_CONFLATE, &confl, sizeof(int));
    socket.connect(subscriber.port);
    socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    port = subscriber.port;
    for(size_t i = 0; i < typeLengths[type]; i++)
        values.push_back(0.0);
}

void Subscriber::readMessage() {
    zmq::message_t message;
    socket.recv(&message);
 
    int numValues = message.size() / sizeof(double);
    assert(numValues == typeLengths[type]);

    for(int i = 0; i < numValues; i++)
        values[i] = *(reinterpret_cast<double*>(message.data()) + i);

}

void Subscriber::setDataType(CommsDataType dataType) {
    type = dataType;
    values.resize(typeLengths[dataType]);
    for(size_t i = 0; i < typeLengths[type]; i++)
        values[i] = 0.0;
}