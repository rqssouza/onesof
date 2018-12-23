#ifndef SOCKET_H
#define SOCKET_H

#include <zmqpp/zmqpp.hpp>
#include <string>

#include "Message.h"

namespace message
{
class Socket
{
public:
    Socket(zmqpp::context& context , zmqpp::socket_type type, const std::string& address);
    ~Socket(){}

    void Subscribe(const Message& message);
    void Unsubscribe(const Message& message);
    void Send(const Message& message);
    bool Receive(Message& message);

private:
    zmqpp::socket m_socket;
};
}
#endif
