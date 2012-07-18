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

namespace cdaq {
    
} //namespace cdaq

#endif //ifndef CDAQ_MISC_DATE_HH
