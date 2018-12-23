#ifndef CLIENT_BUILDER_H
#define CLIENT_BUILDER_H

#include <string>

#include "Client.h"

using namespace std;

namespace message
{
class ClientBuilder
{
public:
    ClientBuilder();
    ~ClientBuilder(){}
    std::unique_ptr<Client> MakeClient(const string& frontendAddr, const string& backendAddr);
private:
    zmqpp::context _context;
};
}
#endif
