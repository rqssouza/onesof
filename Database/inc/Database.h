#ifndef DATABASE_H
#define DATABASE_H

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>


using namespace std;

namespace business
{
class Transaction;
class DateHourQuery;
}

namespace database
{
class Database
{
public:
    Database(const string& uri_str);
    ~Database();
    void Insert(const vector<business::Transaction>& transactions);
    void FindAll(vector<business::Transaction>& transactions);
    void FindByAskBroker(vector<business::Transaction>& transactions, int32_t value);
    void FindByBidBroker(vector<business::Transaction>& transactions, int32_t value);
    void FindBySessionDate(vector<business::Transaction> &transactions, business::DateHourQuery& interval);

private:
    bsoncxx::document::value transactionToDocument(const business::Transaction& transaction);
    void documentToTransaction(const bsoncxx::document::view& document, business::Transaction& transaction);
    void find(const bsoncxx::document::view_or_value& query, vector<business::Transaction>& transactions);

    const string SESSION_DATE_HOUR;
    const string SYMBOL;
    const string BUSINESS_NUMBER;
    const string BUSINESS_PRICE;
    const string QUANTITY;
    const string ANNULMENT;
    const string BID_OFFER_DATE;
    const string BID_OFFER_SEQ;
    const string BID_OFFER_GEN_ID;
    const string BID_OFFER_COND;
    const string ASK_OFFER_DATE;
    const string ASK_OFFER_SEQ;
    const string ASK_OFFER_GEN_ID;
    const string ASK_OFFER_COND;
    const string DIRECT_IND;
    const string BID_BROKER;
    const string ASK_BROKER;

    mongocxx::instance _instance;
    mongocxx::uri _uri;
    mongocxx::client _client;
    mongocxx::database _db;
    mongocxx::collection _collection;
};
}

#endif
