FIND_PACKAGE(Boost REQUIRED
            )
INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    )

ADD_LIBRARY(cdaqImage ${CDAQ_LIBRARY_TYPE}
    image.cpp image.hh
    image_opencv.cpp image_opencv.hh
    )
    
TARGET_LINK_LIBRARIES(cdaqImage
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    )

CDAQ_INSTALL_LIB(cdaqImage)
