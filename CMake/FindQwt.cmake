FIND_PATH(Qwt_INCLUDE_DIRS qwt.h 
    HINTS $ENV{QWT_ROOT}
    PATH_SUFFIXES include qwt/include qwt6/include
    ENV PATH )

FIND_LIBRARY(Qwt_LIBRARY_RELEASE 
    NAMES qwt 
    HINTS $ENV{QWT_ROOT}
    PATH_SUFFIXES lib qwt/lib qwt6/lib
    ENV PATH )

FIND_LIBRARY(Qwt_LIBRARY_DEBUG
    NAMES qwtd
    HINTS $ENV{QWT_ROOT}
    PATH_SUFFIXES lib qwt/lib qwt6/lib
    ENV PATH )

SET(Qwt_LIBRARIES
    optimized ${Qwt_LIBRARY_RELEASE} 
    debug ${Qwt_LIBRARY_DEBUG}
    )
