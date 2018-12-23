#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include <sstream>
#include <queue>
#include <map>
#include <iostream>
#include <stdexcept>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

#include "Socket.h"

using namespace std;

namespace message
{
class Client
{
public:
    enum MessagesID
    {
        Empty,
        TransactionsToStore,
        TransactionsStored,
        TransactionsByBidBrokerQuery,
        TransactionsByBidBrokerReply,
        TransactionsByAskBrokerQuery,
        TransactionsByAskBrokerReply,
        TransactionsBySessionDateQuery,
        TransactionsBySessionDateReply
    };

    Client(unique_ptr<Socket> frontend, unique_ptr<Socket> backend):
            _frontend(move(frontend)),
            _backend(move(backend)),
            _buffer()
    {
    }

    ~Client()
    {
    }

    void Subscribe(MessagesID type)
    {
        _buffer.insert(std::pair<int, SubBuffer>(type, SubBuffer()));
        Message message(type, "");

        try
        {
            _frontend->Subscribe(message);
        }
        catch(const std::exception& e)
        {
            std::stringstream ss;
            ss << "MessageClient: Error subscribing to type '" << type << "' : " << e.what();
            throw std::runtime_error(ss.str());
        }
    }

    void Unsubscribe(int type)
    {
        _buffer.erase(type);
        Message message(type, "");

        try
        {
            _frontend->Unsubscribe(message);
        }
        catch(const std::exception& e)
        {
            std::stringstream ss;
            ss << "MessageClient: Error unsubscribing to type '" << type << "' : " << e.what();
            throw std::runtime_error(ss.str());
        }
    }

    template<typename T>
    void Send(const T& msg, MessagesID type)
    {
        std::ostringstream snd;
        boost::archive::text_oarchive oarchive(snd);
        oarchive << msg;
        Message message(type, snd.str());

        try
        {
            _backend->Send(message);
        }
        catch( const std::exception& e )
        {
            std::stringstream ss;
            ss << "MessageClient: Error sending message '" << message.ToString() << "' : " << e.what();
            throw std::runtime_error(ss.str());
        }
    }

    template<typename T>
    std::queue<T> GetMessages(MessagesID type)
    {
        std::queue<T> ret;
        std::map<int, SubBuffer>::iterator subBuffer = _buffer.find(type);
        if(subBuffer == _buffer.end())
        {
            return ret;
        }

        while(!subBuffer->second.empty())
        {
            T msg;
            std::istringstream ss(subBuffer->second.front().GetBody());
            boost::archive::text_iarchive iarchive(ss);
            iarchive >> msg;
            ret.push(msg);
            subBuffer->second.pop();
        }

        return ret;
    }

    void ProcessMessages()
    {
        Message message;
        std::queue<Message> invalidHeaders;
        try
        {
            while(_frontend->Receive(message))
            {
                std::map<int, SubBuffer>::iterator subBuffer = _buffer.find(message.GetHead());
                if(subBuffer == _buffer.end())
                {
                    invalidHeaders.push(message);
                    continue;
                }

                subBuffer->second.push(message);
            }
        }
        catch(const std::exception& e)
        {
            std::stringstream ss;
            ss << "MessageClient: Error reading message from message broker: " << e.what();
            throw std::runtime_error(ss.str());
        }

        if(!invalidHeaders.empty())
        {
            std::stringstream ss;
            while(!invalidHeaders.empty())
            {
                ss << "MessageClient: Got message with unknown type: '" << invalidHeaders.front().ToString() << "'" << std::endl;
                invalidHeaders.pop();
            }
            throw std::runtime_error(ss.str());
        }
    }

private:
    typedef std::queue<Message> SubBuffer;

    unique_ptr<Socket> _frontend;
    unique_ptr<Socket> _backend;
    std::map<int, SubBuffer> _buffer;
};
}

#endif
