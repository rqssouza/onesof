#ifndef CONVERSION_HELPER_H
#define CONVERSION_HELPER_H

#include <chrono>
#include <string>

using namespace std;

namespace business
{
class ConversionHelper
{
public:
    static tm DateFromMilliseconds(const chrono::milliseconds& ms, chrono::milliseconds& reminder);
    static chrono::milliseconds MillisecondsFromDate(const string& date, const string& hour);
private:
    ConversionHelper(){}
};
}

#endif
