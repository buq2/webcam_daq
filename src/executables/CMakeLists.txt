ADD_EXECUTABLE(cdaq_daq_test
    main_daq_test.cpp
    )
TARGET_LINK_LIBRARIES(cdaq_daq_test
    cdaqDaq
    cdaqDate
    )

ADD_EXECUTABLE(cdaq_cam_test
    main_cam_test.cpp
    )
TARGET_LINK_LIBRARIES(cdaq_cam_test
    cdaqCamera
    cdaqDate
	cdaqImage
    )
