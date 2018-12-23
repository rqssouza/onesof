#include "Socket.h"

namespace message
{
Socket::Socket(zmqpp::context& context , zmqpp::socket_type type, const std::string& address):
        m_socket(context, type)
{
    m_socket.connect(address);
}

void Socket::Subscribe(const Message& message)
{
    m_socket.subscribe(message.GetHeadText());
}

void Socket::Unsubscribe(const Message& message)
{
    m_socket.unsubscribe(message.GetHeadText());
}

void Socket::Send(const Message& message)
{
    zmqpp::message messages;
    messages << message.GetHeadText();
    messages << message.GetBody();
    m_socket.send(messages);
}

bool Socket::Receive(Message& message)
{
    std::string header;
    std::string body;

    if(!m_socket.receive(header, true))
    {
        return false;
    }

    header.pop_back();

    m_socket.receive(body);
    message.SetHead(std::stoi(header));
    message.SetBody(body);
    return true;
}
}
