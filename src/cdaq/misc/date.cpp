#include "cdaq/misc/date.hh"
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/date_time.hpp>
#include <sstream>

using namespace cdaq;

Date::Date()
    :
    date_(boost::posix_time::microsec_clock::local_time())
{
}

Date::~Date()
{
}

std::string Date::ToString(const std::string &format) const
{
    boost::posix_time::time_facet *time_facet(new boost::posix_time::time_facet);
    time_facet->format(format.c_str());
    std::stringstream str_stream;
    str_stream.imbue(std::locale(std::locale::classic(), time_facet));
    str_stream << date_;
    return str_stream.str();
}

Date Date::Now()
{
    return Date();
}

void Date::AddMicroseconds(const boost::int64_t &add_microseconds)
{
    date_ = date_ + boost::posix_time::microseconds(add_microseconds);
}

boost::uint64_t Date::Microseconds() const
{
    const boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1)); 
    return (date_-epoch).total_microseconds();
}

