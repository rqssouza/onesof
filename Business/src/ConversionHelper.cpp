#include "ConversionHelper.h"

#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <ctime>

namespace business
{
tm ConversionHelper::DateFromMilliseconds(const chrono::milliseconds& ms, chrono::milliseconds& reminder)
{
    chrono::time_point<chrono::high_resolution_clock, chrono::milliseconds> tpm(ms);
    chrono::time_point<chrono::high_resolution_clock, chrono::seconds> tps(chrono::time_point_cast<chrono::seconds>(tpm));
    reminder = tpm - chrono::time_point_cast<chrono::seconds>(tps);
    time_t timet = chrono::system_clock::to_time_t(tps);
    struct tm ret = *gmtime(&timet);
    return ret;
}

chrono::milliseconds ConversionHelper::MillisecondsFromDate(const string& date, const string& hour)
{
    vector<string> hour_mseconds;
    boost::split(hour_mseconds, hour, [](char c){return c == '.';});
    chrono::milliseconds ret;
    tm tm{};
    stringstream ss;
    ss << date << " " << hour_mseconds[0];
    ss >> get_time(&tm, "%Y-%m-%d %H:%M:%S");
    ret = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::from_time_t(mktime(&tm)).time_since_epoch());
    ret += chrono::milliseconds(stoi(hour_mseconds[1]));
    return ret;
}
}
