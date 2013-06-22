#include "cdaq/misc/date.hh"

using namespace cdaq;

Date::Date()
{
}

Date::~Date()
{
}

std::string Date::ToString(const std::string &date_format) const
{
    return "test";
}

Date Date::Now()
{
    return Date();
}
