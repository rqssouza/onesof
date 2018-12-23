#include <sstream>
#include <iostream>

#include "Database.h"
#include "Transaction.h"
#include "DateHourQuery.h"

namespace database
{
Database::Database(const string& uri_str):
        SESSION_DATE_HOUR("sessionDateHour"),
        SYMBOL("symbol"),
        BUSINESS_NUMBER("businessNumber"),
        BUSINESS_PRICE("businessPrice"),
        QUANTITY("quantity"),
        ANNULMENT("annulment"),
        BID_OFFER_DATE("bidOfferDate"),
        BID_OFFER_SEQ("bidOfferSeq"),
        BID_OFFER_GEN_ID("bidOfferGenID"),
        BID_OFFER_COND("bidOfferCond"),
        ASK_OFFER_DATE("askOfferDate"),
        ASK_OFFER_SEQ("askOfferSeq"),
        ASK_OFFER_GEN_ID("askOfferGenID"),
        ASK_OFFER_COND("askOfferCond"),
        DIRECT_IND("directInd"),
        BID_BROKER("bidBroker"),
        ASK_BROKER("askBroker"),
        _instance(),
        _uri(uri_str),
        _client(_uri),
        _db(_client["market_data"]),
        _collection(_db["DOLF18"])
{
}


Database::~Database()
{
}

void Database::Insert(const vector<business::Transaction>& transactions)
{
    std::vector<bsoncxx::document::value> documents;
    for(const auto& transaction : transactions)
        documents.push_back(transactionToDocument(transaction));

    bsoncxx::stdx::optional<mongocxx::result::insert_many> result = _collection.insert_many(documents);
    if(result.value().inserted_count() != static_cast<int32_t>(documents.size()))
    {
        stringstream ss;
        ss << "Fail inserting documents into database: " << result.value().inserted_count() << " | " << documents.size();
        throw runtime_error(ss.str());
    }
}

bsoncxx::document::value Database::transactionToDocument(const business::Transaction& transaction)
{
    return bsoncxx::builder::stream::document{}
           << SESSION_DATE_HOUR << bsoncxx::types::b_date(transaction.sessionDateHour())
           << SYMBOL << transaction.symbol()
           << BUSINESS_NUMBER << transaction.businessNumber()
           << BUSINESS_PRICE << transaction.businessPrice()
           << QUANTITY << transaction.quantity()
           << ANNULMENT << transaction.annulment()
           << BID_OFFER_DATE << bsoncxx::types::b_date(transaction.bidOfferDate())
           << BID_OFFER_SEQ << transaction.bidOfferSeq()
           << BID_OFFER_GEN_ID << transaction.bidOfferGenID()
           << BID_OFFER_COND << transaction.bidOfferCond()
           << ASK_OFFER_DATE << bsoncxx::types::b_date(transaction.askOfferDate())
           << ASK_OFFER_SEQ << transaction.askOfferSeq()
           << ASK_OFFER_GEN_ID << transaction.askOfferGenID()
           << ASK_OFFER_COND << transaction.askOfferCond()
           << DIRECT_IND << transaction.directInd()
           << BID_BROKER << transaction.bidBroker()
           << ASK_BROKER << transaction.askBroker()
           << bsoncxx::builder::stream::finalize;
}

void Database::documentToTransaction(const bsoncxx::document::view& document, business::Transaction& transaction)
{
    string symbol(document[SYMBOL].get_utf8().value);
    transaction.sessionDateHour(document[SESSION_DATE_HOUR].get_date().value);
    transaction.symbol(symbol);
    transaction.businessNumber(document[BUSINESS_NUMBER].get_int64().value);
    transaction.businessPrice(document[BUSINESS_PRICE].get_double().value);
    transaction.quantity(document[QUANTITY].get_int64().value);
    transaction.annulment(document[ANNULMENT].get_int32().value);
    transaction.bidOfferDate(document[BID_OFFER_DATE].get_date().value);
    transaction.bidOfferSeq(document[BID_OFFER_SEQ].get_int64().value);
    transaction.bidOfferGenID(document[BID_OFFER_GEN_ID].get_int64().value);
    transaction.bidOfferCond(document[BID_OFFER_COND].get_int32().value);
    transaction.askOfferDate(document[ASK_OFFER_DATE].get_date().value);
    transaction.askOfferSeq(document[ASK_OFFER_SEQ].get_int64().value);
    transaction.askOfferGenID(document[ASK_OFFER_GEN_ID].get_int64().value);
    transaction.askOfferCond(document[ASK_OFFER_COND].get_int32().value);
    transaction.directInd(document[DIRECT_IND].get_int32().value);
    transaction.bidBroker(document[BID_BROKER].get_int32().value);
    transaction.askBroker(document[ASK_BROKER].get_int32().value);
}

void Database::find(const bsoncxx::document::view_or_value& query, vector<business::Transaction>& transactions)
{
    mongocxx::cursor cursor = _collection.find(query);
    for(const bsoncxx::document::view& document : cursor)
    {
        business::Transaction transaction;
        documentToTransaction(document, transaction);
        transactions.push_back(transaction);
    }
}

void Database::FindAll(vector<business::Transaction>& transactions)
{
    find({}, transactions);
}

void Database::FindByAskBroker(vector<business::Transaction>& transactions, int32_t value)
{
    bsoncxx::document::view_or_value query = bsoncxx::builder::stream::document{}
                                             << ASK_BROKER << value
                                             << bsoncxx::builder::stream::finalize;
    find(query, transactions);
}

void Database::FindByBidBroker(vector<business::Transaction>& transactions, int32_t value)
{
    bsoncxx::document::view_or_value query = bsoncxx::builder::stream::document{}
                                             << BID_BROKER << value
                                             << bsoncxx::builder::stream::finalize;
    find(query, transactions);
}

void Database::FindBySessionDate(vector<business::Transaction> &transactions, business::DateHourQuery& interval)
{
    bsoncxx::document::view_or_value query = bsoncxx::builder::stream::document{}
                                             << SESSION_DATE_HOUR
                                             << bsoncxx::builder::stream::open_document
                                                << "$gte" << bsoncxx::types::b_date(interval.date0())
                                                << "$lte" << bsoncxx::types::b_date(interval.date1())
                                             << bsoncxx::builder::stream::close_document
                                             << bsoncxx::builder::stream::finalize;
    find(query, transactions);
}
}
