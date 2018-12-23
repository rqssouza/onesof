#include "ClientBuilder.h"
#include "Socket.h"

namespace message
{
ClientBuilder::ClientBuilder():
        _context()
{
}

unique_ptr<Client> ClientBuilder::MakeClient(const string& frontendAddr, const string& backendAddr)
{
    return make_unique<Client>(make_unique<Socket>(_context, zmqpp::socket_type::subscribe, frontendAddr),
                               make_unique<Socket>(_context, zmqpp::socket_type::publish, backendAddr));
}
}
