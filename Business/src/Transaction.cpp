#include "Transaction.h"

#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <ctime>

#include "ConversionHelper.h"

namespace business
{
Transaction::Transaction():
    _sessionDateHour(0),
    _symbol(""),
    _businessNumber(0),
    _businessPrice(0.0),
    _quantity(0),
    _annulment(0),
    _bidOfferDate(0),
    _bidOfferSeq(0),
    _bidOfferGenID(0),
    _bidOfferCond(0),
    _askOfferDate(0),
    _askOfferSeq(0),
    _askOfferGenID(0),
    _askOfferCond(0),
    _directInd(0),
    _bidBroker(0),
    _askBroker(0)
{
}

Transaction::Transaction(const Transaction& other):
    _sessionDateHour(other._sessionDateHour),
    _symbol(other._symbol),
    _businessNumber(other._businessNumber),
    _businessPrice(other._businessPrice),
    _quantity(other._quantity),
    _annulment(other._annulment),
    _bidOfferDate(other._bidOfferDate),
    _bidOfferSeq(other._bidOfferSeq),
    _bidOfferGenID(other._bidOfferGenID),
    _bidOfferCond(other._bidOfferCond),
    _askOfferDate(other._askOfferDate),
    _askOfferSeq(other._askOfferSeq),
    _askOfferGenID(other._askOfferGenID),
    _askOfferCond(other._askOfferCond),
    _directInd(other._directInd),
    _bidBroker(other._bidBroker),
    _askBroker(other._askBroker)
{
}

Transaction& Transaction::operator=(const Transaction& other)
{
    if(&other == this)
        return *this;
    _sessionDateHour = other._sessionDateHour;
    _symbol = other._symbol;
    _businessNumber = other._businessNumber;
    _businessPrice = other._businessPrice;
    _quantity = other._quantity;
    _annulment = other._annulment;
    _bidOfferDate = other._bidOfferDate;
    _bidOfferSeq = other._bidOfferSeq;
    _bidOfferGenID = other._bidOfferGenID;
    _bidOfferCond = other._bidOfferCond;
    _askOfferDate = other._askOfferDate;
    _askOfferSeq = other._askOfferSeq;
    _askOfferGenID = other._askOfferGenID;
    _askOfferCond = other._askOfferCond;
    _directInd = other._directInd;
    _bidBroker = other._bidBroker;
    _askBroker = other._askBroker;
    return *this;
}

Transaction::Transaction(const string& row)
{
    vector<string> fields;
    boost::split(fields, row, [](char c){return c == ';';});
    _sessionDateHour = ConversionHelper::MillisecondsFromDate(fields[SESSION_DATE], fields[SESSION_HOUR]);
    _symbol = fields[SYMBOL];
    _businessNumber = static_cast<int64_t>(stoll(fields[BUSINESS_NUMBER]));
    _businessPrice = stod(fields[BUSINESS_PRICE]);
    _quantity = static_cast<int64_t>(stoll(fields[QUANTITY]));
    _annulment = stoi(fields[ANNULMENT]);
    _bidOfferDate = ConversionHelper::MillisecondsFromDate(fields[BID_OFFER_DATE], "00:00:00.000");
    _bidOfferSeq = static_cast<int64_t>(stoll(fields[BID_OFFER_SEQ]));
    _bidOfferGenID = static_cast<int64_t>(stoll(fields[BID_OFFER_GEN_ID]));
    _bidOfferCond = stoi(fields[BID_OFFER_COND]);
    _askOfferDate = ConversionHelper::MillisecondsFromDate(fields[ASK_OFFER_DATE], "00:00:00.000");
    _askOfferSeq = static_cast<int64_t>(stoll(fields[ASK_OFFER_SEQ]));
    _askOfferGenID = static_cast<int64_t>(stoll(fields[ASK_OFFER_GEN_ID]));
    _askOfferCond = stoi(fields[ASK_OFFER_COND]);
    _directInd = stoi(fields[DIRECT_IND]);
    _bidBroker = stoi(fields[BID_BROKER]);
    _askBroker = stoi(fields[ASK_BROKER]);
}

Transaction::~Transaction()
{
}

string Transaction::Format() const
{
    stringstream ss;
    chrono::milliseconds session_ms;
    chrono::milliseconds ms;
    struct tm session_t = ConversionHelper::DateFromMilliseconds(_sessionDateHour, session_ms);
    struct tm bid_t = ConversionHelper::DateFromMilliseconds(_sessionDateHour, ms);
    struct tm ask_t = ConversionHelper::DateFromMilliseconds(_sessionDateHour, ms);
    ss << setfill('0') << setw(4) << session_t.tm_year + 1900 << "-"
       << setfill('0') << setw(2) << session_t.tm_mon + 1 << "-"
       << setfill('0') << setw(2) << session_t.tm_mday << ";";
    ss << _symbol << ";";
    ss << setfill('0') << setw(10) << _businessNumber << ";";
    ss << " " << setfill('0') << setw(19) << fixed << setprecision(6) << _businessPrice << ";";
    ss << setfill('0') << setw(18) << _quantity << ";";
    ss << setfill('0') << setw(2) << session_t.tm_hour << ":"
       << setfill('0') << setw(2) << session_t.tm_min << ":"
       << setfill('0') << setw(2) << session_t.tm_sec << "."
       << setfill('0') << setw(3) << session_ms.count() << ";";
    ss << setfill('0') << setw(1) << _annulment << ";";
    ss << setfill('0') << setw(4) << bid_t.tm_year + 1900 << "-"
       << setfill('0') << setw(2) << bid_t.tm_mon + 1 << "-"
       << setfill('0') << setw(2) << bid_t.tm_mday << ";";
    ss << setfill('0') << setw(15) << _bidOfferSeq << ";";
    ss << setfill('0') << setw(15) << _bidOfferGenID << ";";
    ss << setfill('0') << setw(1) << _bidOfferCond << ";";
    ss << setfill('0') << setw(4) << ask_t.tm_year + 1900 << "-"
       << setfill('0') << setw(2) << ask_t.tm_mon + 1 << "-"
       << setfill('0') << setw(2) << ask_t.tm_mday << ";";
    ss << setfill('0') << setw(15) << _askOfferSeq << ";";
    ss << setfill('0') << setw(15) << _askOfferGenID << ";";
    ss << setfill('0') << setw(1) << _askOfferCond << ";";
    ss << setfill('0') << setw(1) << _directInd << ";";
    ss << setfill('0') << setw(8) << _bidBroker << ";";
    ss << setfill('0') << setw(8) << _askBroker;
    return ss.str();
}
}
