FIND_PACKAGE(Boost REQUIRED
            COMPONENTS
            date_time
            )
INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    )

ADD_LIBRARY(cdaqDate ${CDAQ_LIBRARY_TYPE}
    date.cpp date.hh
    )
    
TARGET_LINK_LIBRARIES(cdaqDate
    ${Boost_DATE_TIME_LIBRARY}
    )

CDAQ_INSTALL_LIB(cdaqDate)
