#ifndef PROXY_H
#define PROXY_H

#include <string>
#include "zmqpp/zmqpp.hpp"

namespace message
{
class Proxy
{
public:
    Proxy(zmqpp::context& context, const std::string& frontEndAddress, const std::string& backEndAddress);
    ~Proxy();

    void Run();
private:
    zmqpp::socket _frontend;
    zmqpp::socket _backend;
    zmqpp::poller _poller;
};
}
#endif
