#include "DateHourQuery.h"
#include "ConversionHelper.h"

namespace business
{
DateHourQuery::DateHourQuery():
    _date0(0),
    _date1(0)
{
}

DateHourQuery::DateHourQuery(const string& date0, const string& hour0, const string& date1, const string& hour1):
    _date0(ConversionHelper::MillisecondsFromDate(date0, hour0)),
    _date1(ConversionHelper::MillisecondsFromDate(date1, hour1))
{
}

DateHourQuery::DateHourQuery(const DateHourQuery& other):
    _date0(other._date0),
    _date1(other._date1)
{
}
}
