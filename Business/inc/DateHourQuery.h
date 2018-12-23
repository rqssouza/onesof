#ifndef DATE_HOUR_QUERY_H
#define DATE_HOUR_QUERY_H

#include <chrono>
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/binary_object.hpp>

using namespace std;

namespace business
{
class DateHourQuery
{
public:
    DateHourQuery();
    DateHourQuery(const string& date0, const string& hour0, const string& date1, const string& hour1);
    DateHourQuery(const DateHourQuery& other);
    ~DateHourQuery(){}

    DateHourQuery& operator=(const DateHourQuery& other);

    chrono::milliseconds date0() const { return _date0; }
    void date0(chrono::milliseconds val){ _date0 = val; }

    chrono::milliseconds date1() const { return _date1; }
    void date1(chrono::milliseconds val){ _date1 = val; }
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int)
    {
        ar & boost::serialization::make_binary_object(&_date0, sizeof(_date0));
        ar & boost::serialization::make_binary_object(&_date1, sizeof(_date1));
    }
    chrono::milliseconds _date0;
    chrono::milliseconds _date1;
};
}

#endif
