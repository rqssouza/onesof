#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <unistd.h>
#include <boost/program_options.hpp>

#include "Transaction.h"
#include "Database.h"
#include "ClientBuilder.h"

using namespace std;

namespace pc = boost::program_options;

int main(int argc, char* argv[])
{
    try
    {
        pc::options_description desc("Options");
        desc.add_options()
             ("help", "produce help message")
             ("frontend", pc::value<string>()->default_value("localhost:5556"), "set frontend address.")
             ("backend", pc::value<string>()->default_value("localhost:5555"), "set backend address.");

        pc::variables_map vm;
        pc::store(pc::parse_command_line(argc, argv, desc), vm);
        pc::notify(vm);

        if(vm.count("help"))
        {
            cout << desc << endl;
            return 0;
        }

        message::ClientBuilder clientBuilder;
        unique_ptr<message::Client> client = clientBuilder.MakeClient("tcp://" + vm["frontend"].as<string>(), "tcp://" + vm["backend"].as<string>());
        client->Subscribe(message::Client::TransactionsStored);
        while(true)
        {
            try
            {
                client->ProcessMessages();
                queue<vector<business::Transaction>> messages = client->GetMessages<vector<business::Transaction>>(message::Client::TransactionsStored);
                while(!messages.empty())
                {
                    for(auto& transaction : messages.front())
                        cout << transaction.Format() << endl;
                    messages.pop();
                }
                usleep(1000);
            }
            catch(const exception& e)
            {
                cerr << "subscribed_client error: " << e.what() << endl;
            }
        }
    }
    catch(const exception& e)
    {
        cerr << "subscribed_client fatal error!: " << e.what() << endl;
        return -1;
    }

    catch(...)
    {
        cerr << "subscribed_client unknown fatal error!" << endl;
        return -1;
    }
}
