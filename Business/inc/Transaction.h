#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <chrono>
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/binary_object.hpp>

using namespace std;

namespace business
{
class Transaction
{
public:
    Transaction();
    Transaction(const string& row);
    Transaction(const Transaction& other);
    ~Transaction();

    Transaction& operator=(const Transaction& other);
    string Format() const;

    chrono::milliseconds sessionDateHour() const { return _sessionDateHour; }
    void sessionDateHour(chrono::milliseconds val){ _sessionDateHour = val; }

    const string& symbol() const { return _symbol; }
    void symbol(string val){ _symbol = val; }

    int64_t businessNumber() const { return _businessNumber; }
    void businessNumber(int64_t val){ _businessNumber = val; }

    double businessPrice() const { return _businessPrice; }
    void businessPrice(double val){ _businessPrice = val; }

    int64_t quantity() const { return _quantity; }
    void quantity(int64_t val){ _quantity = val; }

    int32_t annulment() const { return _annulment; }
    void annulment(int32_t val){ _annulment = val; }

    chrono::milliseconds bidOfferDate() const { return _bidOfferDate; }
    void bidOfferDate(chrono::milliseconds val){ _bidOfferDate = val; }

    int64_t bidOfferSeq() const { return _bidOfferSeq; }
    void bidOfferSeq(int64_t val){ _bidOfferSeq = val; }

    int64_t bidOfferGenID() const { return _bidOfferGenID; }
    void bidOfferGenID(int64_t val){ _bidOfferGenID = val; }

    int32_t bidOfferCond() const { return _bidOfferCond; }
    void bidOfferCond(int32_t val){ _bidOfferCond = val; }

    chrono::milliseconds askOfferDate() const { return _askOfferDate; }
    void askOfferDate(chrono::milliseconds val){ _askOfferDate = val; }

    int64_t askOfferSeq() const { return _askOfferSeq; }
    void askOfferSeq(int64_t val){ _askOfferSeq = val; }

    int64_t askOfferGenID() const { return _askOfferGenID; }
    void askOfferGenID(int64_t val){ _askOfferGenID = val; }

    int32_t askOfferCond() const { return _askOfferCond; }
    void askOfferCond(int32_t val){ _askOfferCond = val; }

    int32_t directInd() const { return _directInd; }
    void directInd(int32_t val){ _directInd = val; }

    int32_t bidBroker() const { return _bidBroker; }
    void bidBroker(int32_t val){ _bidBroker = val; }

    int32_t askBroker() const { return _askBroker; }
    void askBroker(int32_t val){ _askBroker = val; }
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int)
    {
        ar & boost::serialization::make_binary_object(&_sessionDateHour, sizeof(_sessionDateHour));
        ar & _symbol;
        ar & _businessNumber;
        ar & _businessPrice;
        ar & _quantity;
        ar & _annulment;
        ar & boost::serialization::make_binary_object(&_bidOfferDate, sizeof(_bidOfferDate));
        ar & _bidOfferSeq;
        ar & _bidOfferGenID;
        ar & _bidOfferCond;
        ar & boost::serialization::make_binary_object(&_askOfferDate, sizeof(_askOfferDate));
        ar & _askOfferSeq;
        ar & _askOfferGenID;
        ar & _askOfferCond;
        ar & _directInd;
        ar & _bidBroker;
        ar & _askBroker;

    }
    enum Fields
    {
        SESSION_DATE = 0,
        SYMBOL = 1,
        BUSINESS_NUMBER = 2,
        BUSINESS_PRICE = 3,
        QUANTITY = 4,
        SESSION_HOUR = 5,
        ANNULMENT = 6,
        BID_OFFER_DATE = 7,
        BID_OFFER_SEQ = 8,
        BID_OFFER_GEN_ID = 9,
        BID_OFFER_COND = 10,
        ASK_OFFER_DATE = 11,
        ASK_OFFER_SEQ = 12,
        ASK_OFFER_GEN_ID = 13,
        ASK_OFFER_COND = 14,
        DIRECT_IND = 15,
        BID_BROKER = 16,
        ASK_BROKER = 17

    };
    chrono::milliseconds _sessionDateHour;
    string _symbol;
    int64_t _businessNumber;
    double _businessPrice;
    int64_t _quantity;
    int32_t _annulment;
    chrono::milliseconds _bidOfferDate;
    int64_t _bidOfferSeq;
    int64_t _bidOfferGenID;
    int32_t _bidOfferCond;
    chrono::milliseconds _askOfferDate;
    int64_t _askOfferSeq;
    int64_t _askOfferGenID;
    int32_t _askOfferCond;
    int32_t _directInd;
    int32_t _bidBroker;
    int32_t _askBroker;
};
}

#endif
