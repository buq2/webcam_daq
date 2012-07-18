FIND_PACKAGE(OpenCV REQUIRED
            )
INCLUDE_DIRECTORIES(
    ${OpenCV_INCLUDE_DIRS}
    )

#Image library used for testing
ADD_LIBRARY(cdaqCamera ${CDAQ_LIBRARY_TYPE}
    camera.cpp camera.hh
    )
    
#Image library uses Devil
TARGET_LINK_LIBRARIES(cdaqCamera
    ${OpenCV_LIBS}
    )
    
CDAQ_INSTALL_LIB(cdaqCamera)
