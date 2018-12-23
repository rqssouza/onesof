#include <sstream>
#include <iostream>
#include <unistd.h>
#include <boost/program_options.hpp>

#include "Proxy.h"

using namespace std;

namespace pc = boost::program_options;

int main(int argc, char* argv[])
{
    try
    {
        pc::options_description desc("Options");
        desc.add_options()
             ("help", "produce help message")
             ("frontend", pc::value<string>()->default_value("5555"), "set listening frontend port.")
             ("backend", pc::value<string>()->default_value("5556"), "set listening backend port.");

        pc::variables_map vm;
        pc::store(pc::parse_command_line(argc, argv, desc), vm);
        pc::notify(vm);

        if(vm.count("help"))
        {
            cout << desc << endl;
            return 0;
        }

        zmqpp::context context;
        message::Proxy proxy(context, "tcp://*:" + vm["frontend"].as<string>(), "tcp://*:" + vm["backend"].as<string>());
        while( true )
        {
            try
            {
                proxy.Run();
                usleep(1000);
            }
            catch( const std::exception& e )
            {
                cerr << "messageBroker error: " << e.what() << endl;
            }
        }
    }
    catch( const std::exception& e )
    {
        cerr << "messageBroker fatal error!: " << e.what() << endl;
        return -1;
    }
    catch(...)
    {
        cerr << "messageBroker: unknown fatal error!" << endl;
        return -1;
    }
}
