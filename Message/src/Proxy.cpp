#include <zmqpp/zmqpp.hpp>

#include "Proxy.h"

namespace message
{
Proxy::Proxy(zmqpp::context& context, const std::string& frontEndAddress, const std::string& backEndAddress) :
        _frontend(context, zmqpp::socket_type::xsubscribe),
        _backend(context, zmqpp::socket_type::xpublish),
        _poller()
{
    _frontend.bind(frontEndAddress);
    _backend.bind(backEndAddress);

    _poller.add(_frontend);
    _poller.add(_backend);
}

Proxy::~Proxy()
{
}

void Proxy::Run()
{
    _poller.poll(0);
    std::string message;
    if(_poller.events(_frontend) == zmqpp::poller::poll_in)
    {
        zmqpp::message messages;
        bool more = true;
        while(more)
        {
            _frontend.receive(message);
            more = _frontend.has_more_parts();
            messages << message;
        }
        _backend.send(messages);
    }

    if(_poller.events(_backend) == zmqpp::poller::poll_in)
    {
        zmqpp::message messages;
        bool more = true;
        while(more)
        {
            _backend.receive(message);
            more = _backend.has_more_parts();
            messages << message;
        }
        _frontend.send(messages);
    }
}
}
