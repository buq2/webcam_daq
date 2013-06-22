ADD_EXECUTABLE(cdaq_daq_test
    main_daq_test.cpp
    )
TARGET_LINK_LIBRARIES(cdaq_daq_test
    cdaqDaq
    cdaqDate
    )
