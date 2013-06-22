#Include current directory to list of "include" directories,
#enables use of "#include "cdaq/camera/camera.hh"" etc.
INCLUDE_DIRECTORIES(.)
ADD_SUBDIRECTORY(cdaq)

OPTION(CDAQ_COMPILE_EXECUTABLES
    "Compile cdaq executables" ON)
IF (CDAQ_COMPILE_EXECUTABLES)
    ADD_SUBDIRECTORY(executables)
ENDIF()
