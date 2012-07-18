FIND_PACKAGE(Boost REQUIRED
            COMPONENTS 
            filesystem
            thread
            system
            )
INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    )

ADD_LIBRARY(cdaqDaq ${CDAQ_LIBRARY_TYPE}
    daq.cpp daq.hh
    )
    
TARGET_LINK_LIBRARIES(cdaqDaq
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    )

CDAQ_INSTALL_LIB(cdaqDaq)
