#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <unistd.h>
#include <boost/program_options.hpp>

#include "Transaction.h"
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
             ("backend", pc::value<string>()->default_value("localhost:5555"), "set backend address.")
             ("inputFile", pc::value<string>()->default_value("DOLF18.txt"), "set input file.");

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
        queue<business::Transaction> transactions;
        ifstream file(vm["inputFile"].as<string>());
        while(file)
        {
            string row;
            getline(file, row);
            if(row.empty())
                continue;
            transactions.push(business::Transaction(row));
        }

        chrono::milliseconds now = transactions.front().sessionDateHour();
        auto end = chrono::system_clock::now();
        auto start = end;
        while(!transactions.empty())
        {
            now += chrono::duration_cast<chrono::milliseconds>(end - start);
            vector<business::Transaction> transactionsIn;
            while(now >= transactions.front().sessionDateHour() && !transactions.empty())
            {
                transactionsIn.push_back(transactions.front());
                transactions.pop();
            }

            if(transactionsIn.size())
                client->Send(transactionsIn, message::Client::TransactionsToStore);
            start = end;
            end = chrono::system_clock::now();
            usleep(1000);
        }
    }
    catch(const exception& e)
    {
        cerr << "push_client error: " << e.what() << endl;
        return -1;
    }
    catch(...)
    {
        cerr << "push_client: unknown error" << endl;
        return -1;
    }
    return 0;
}
