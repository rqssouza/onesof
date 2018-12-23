#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <unistd.h>
#include <boost/program_options.hpp>

#include "Transaction.h"
#include "DateHourQuery.h"
#include "ClientBuilder.h"

using namespace std;

namespace pc = boost::program_options;

void PrintMessages(queue<vector<business::Transaction>>& messages)
{
    while(!messages.empty())
    {
        for(auto& transaction : messages.front())
            cout << transaction.Format() << endl;
        messages.pop();
    }
}

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
        bool askBrokerReply = false, bidBrokerReply = false, sessionDateReply = false;
        client->Subscribe(message::Client::TransactionsByAskBrokerReply);
        client->Subscribe(message::Client::TransactionsByBidBrokerReply);
        client->Subscribe(message::Client::TransactionsBySessionDateReply);
        sleep(1);
        client->Send<int32_t>(27, message::Client::TransactionsByAskBrokerQuery);
        client->Send<int32_t>(86, message::Client::TransactionsByBidBrokerQuery);
        client->Send(business::DateHourQuery("2017-12-08", "09:00:00.000", "2017-12-08", "09:10:00.000"), message::Client::TransactionsBySessionDateQuery);
        while(!askBrokerReply || !bidBrokerReply || !sessionDateReply)
        {
            client->ProcessMessages();
            queue<vector<business::Transaction>> askBrokerReplyMessages = client->GetMessages<vector<business::Transaction>>(message::Client::TransactionsByAskBrokerReply);
            queue<vector<business::Transaction>> bidBrokerReplyMessages = client->GetMessages<vector<business::Transaction>>(message::Client::TransactionsByBidBrokerReply);
            queue<vector<business::Transaction>> sessionDateReplyMessages = client->GetMessages<vector<business::Transaction>>(message::Client::TransactionsBySessionDateReply);
            if(!askBrokerReplyMessages.empty())
            {
                askBrokerReply = true;
                cout << "################################# TRANSACTIONS ASK_BROKER = " << 27 << " ##############################" << endl;
                PrintMessages(askBrokerReplyMessages);
            }

            if(!bidBrokerReplyMessages.empty())
            {
                bidBrokerReply = true;
                cout << "################################# TRANSACTIONS BID_BROKER = " << 86 << " ##############################" << endl;
                PrintMessages(bidBrokerReplyMessages);
            }

            if(!sessionDateReplyMessages.empty())
            {
                sessionDateReply = true;
                cout << "############### TRANSACTIONS: " << "2017-12-08 09:00:00.000" << " <= SESSION_DATE <= " << "2017-12-08 09:10:00.000 ##############" << endl;
                PrintMessages(sessionDateReplyMessages);
            }

            usleep(1000);
        }
    }
    catch(const exception& e)
    {
        cerr << "query_client error: " << e.what() << endl;
        return -1;
    }
    catch(...)
    {
        cerr << "query_client: unknown error" << endl;
        return -1;
    }

    return 0;
}
