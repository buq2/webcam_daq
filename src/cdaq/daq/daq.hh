#ifndef CDAQ_DAQ_DAQ_HH
#define CDAQ_DAQ_DAQ_HH

//Macros for exporting/importing functions/etc. These are needed so
//that we can call certain functions in compiled dll/static library.
//Define '*_EXPORTS' is added by CMake.
#if defined (_WIN32) 
#    ifdef cdaqDaq_EXPORTS
         //Creating a library, export (make visible)
#        define CDAQDAQAPI __declspec(dllexport)
#    else
         //Using the library, import (find from library)
#        define CDAQDAQAPI __declspec(dllimport)
#    endif
#else //Not a Windows platform (probably gcc), no need for import/export
      //declarations
#    define CDAQDAQAPI
#endif

namespace cdaq {
    
} //namespace cdaq

#endif //ifndef CDAQ_DAQ_DAQ_HH
