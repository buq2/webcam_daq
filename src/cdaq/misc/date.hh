#ifndef CDAQ_MISC_DATE_HH
#define CDAQ_MISC_DATE_HH

//Macros for exporting/importing functions/etc. These are needed so
//that we can call certain functions in compiled dll/static library.
//Define '*_EXPORTS' is added by CMake.
#if defined (_WIN32) 
#    ifdef cdaqDate_EXPORTS
         //Creating a library, export (make visible)
#        define CDAQDATEAPI __declspec(dllexport)
#    else
         //Using the library, import (find from library)
#        define CDAQDATEAPI __declspec(dllimport)
#    endif
#else //Not a Windows platform (probably gcc), no need for import/export
      //declarations
#    define CDAQDATEAPI
#endif

#include <string>

namespace cdaq {
    
class CDAQDATEAPI Date
{
 public:
    /// Create empty Date object with unknown time
    Date();

    ~Date();
    
    /// Converts Date object to string
    /// \param[in] date_format Format string. For example "%Y-%m-%d %H:%M:%S.%F"
    /// \return Date string.
    /// \see http://www.boost.org/doc/libs/1_49_0/doc/html/date_time/date_time_io.html#date_time.format_flags
    std::string ToString(const std::string &date_format) const;
    
    /// Create date object with current time.
    /// \return Date object with current time.
    static Date Now();
 private:
 private:
}; //class Date
    
} //namespace cdaq

#endif //ifndef CDAQ_MISC_DATE_HH
