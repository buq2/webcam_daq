IF (NOT UNIX)
    #Should fix problem with Qt5 and not finding glu32 (for whatever
    #reason...)
    LIST(APPEND CMAKE_PREFIX_PATH "C:/Program Files/Microsoft SDKs/Windows/v7.1/Lib")    
ENDIF()

FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Concurrent REQUIRED)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Concurrent_INCLUDE_DIRS})
ADD_DEFINITIONS(${QT_DEFINITIONS})

FIND_PACKAGE(Qwt REQUIRED)
INCLUDE_DIRECTORIES(${Qwt_INCLUDE_DIRS})

QT5_WRAP_CPP(cdaqGui_MOC
    gui.hh
    )


ADD_EXECUTABLE(cdaq_gui_test
    main_cdaq_gui.cpp
    gui.hh gui.cpp ${cdaqGui_MOC}
    )
TARGET_LINK_LIBRARIES(cdaq_gui_test
    cdaqCamera
    cdaqDate
    cdaqImage
    cdaqDaq
    ${Qt5Widgets_LIBRARIES}
    ${Qwt_LIBRARIES}
    )
