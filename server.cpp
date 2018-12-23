#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <unistd.h>
#include <boost/program_options.hpp>

#include "Transaction.h"
#include "Database.h"
#include "ClientBuilder.h"
#include "DateHourQuery.h"

using namespace std;

namespace pc = boost::program_options;

void ProcessInput(message::Client& client, database::Database& db)
{
    vector<business::Transaction> transactionsStored;
    queue<vector<business::Transaction>> messages = client.GetMessages<vector<business::Transaction>>(message::Client::TransactionsToStore);
    while(!messages.empty())
    {
        db.Insert(messages.front());
        transactionsStored.insert(transactionsStored.end(), messages.front().begin(), messages.front().end());
        messages.pop();
    }
    client.Send(transactionsStored, message::Client::TransactionsStored);
}

void ProcessQueries(message::Client& client, database::Database& db)
{
    queue<int32_t> byAskBroker = client.GetMessages<int32_t>(message::Client::TransactionsByAskBrokerQuery);
    while(!byAskBroker.empty())
    {
        vector<business::Transaction> result;
        db.FindByAskBroker(result, byAskBroker.front());
        client.Send(result, message::Client::TransactionsByAskBrokerReply);
        byAskBroker.pop();
    }

    queue<int32_t> byBidBroker = client.GetMessages<int32_t>(message::Client::TransactionsByBidBrokerQuery);
    while(!byBidBroker.empty())
    {
        vector<business::Transaction> result;
        db.FindByBidBroker(result, byBidBroker.front());
        client.Send(result, message::Client::TransactionsByBidBrokerReply);
        byBidBroker.pop();
    }

    queue<business::DateHourQuery> bySessionDateHour = client.GetMessages<business::DateHourQuery>(message::Client::TransactionsBySessionDateQuery);
    while(!bySessionDateHour.empty())
    {
        vector<business::Transaction> result;
        db.FindBySessionDate(result, bySessionDateHour.front());
        client.Send(result, message::Client::TransactionsBySessionDateReply);
        bySessionDateHour.pop();
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
             ("backend", pc::value<string>()->default_value("localhost:5555"), "set backend address.")
             ("database", pc::value<string>()->default_value("localhost:27017"), "set database address.");

        pc::variables_map vm;
        pc::store(pc::parse_command_line(argc, argv, desc), vm);
        pc::notify(vm);

        if(vm.count("help"))
        {
            cout << desc << endl;
            return 0;
        }

        database::Database db("mongodb://" + vm["database"].as<string>());
        queue<business::Transaction> transactions;
        message::ClientBuilder clientBuilder;
        unique_ptr<message::Client> client = clientBuilder.MakeClient("tcp://" + vm["frontend"].as<string>(), "tcp://" + vm["backend"].as<string>());
        client->Subscribe(message::Client::TransactionsToStore);
        client->Subscribe(message::Client::TransactionsByAskBrokerQuery);
        client->Subscribe(message::Client::TransactionsByBidBrokerQuery);
        client->Subscribe(message::Client::TransactionsBySessionDateQuery);
        while(true)
        {
            try
            {
                client->ProcessMessages();
                ProcessInput(*client, db);
                ProcessQueries(*client, db);
                usleep(1000);
            }
            catch(const exception& e)
            {
                cerr << "server error: " << e.what() << endl;
            }
        }
    }
    catch(const exception& e)
    {
        cerr << "server fatal error!: " << e.what() << endl;
        return -1;
    }
    catch(...)
    {
        cerr << "server unknown fatal error!" << endl;
        return -1;
    }
}
