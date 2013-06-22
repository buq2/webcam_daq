ADD_LIBRARY(cdaqCamera ${CDAQ_LIBRARY_TYPE}
    camera.cpp camera.hh
    )
    
TARGET_LINK_LIBRARIES(cdaqCamera
    ${OpenCV_LIBS}
    )
    
CDAQ_INSTALL_LIB(cdaqCamera)
