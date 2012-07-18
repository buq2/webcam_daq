cmake_minimum_required (VERSION 2.6)
PROJECT(cdaq C CXX)

#Make sure that all projects have same compilation type
IF (NOT DEFINED CDAQ_BUILD_TYPE)
    SET(CDAQ_BUILD_TYPE  DEBUG)
ENDIF (NOT DEFINED CDAQ_BUILD_TYPE)
SET(CMAKE_BUILD_TYPE ${CDAQ_BUILD_TYPE})

SET(CDAQ_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR})
SET(CDAQ_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
#Define installation path only if they have not been yet defined
#This makes it so that if this cmake project is subproject of another project,
#the freshly compiled libraries/etc are copied to same folder
#as the main projects libraries.
IF (NOT DEFINED CDAQ_BINARY_DIR)
    SET(CDAQ_BINARY_DIR  ${PROJECT_BINARY_DIR}/bin)
ENDIF (NOT DEFINED CDAQ_BINARY_DIR)
IF (NOT DEFINED CDAQ_LIBRARY_DIR)
    SET(CDAQ_LIBRARY_DIR ${PROJECT_BINARY_DIR}/lib)
ENDIF (NOT DEFINED CDAQ_LIBRARY_DIR)
#CMAKE helpers

SET(CMAKE_MODULE_PATH
  "${CDAQ_SOURCE_DIR}/CMake"
  "${CDAQ_SOURCE_DIR}/CMake/modules")
#Includes CMake module installation.cmake
INCLUDE(installation)

# #Application needs to be build with /MT(d) flag
# IF(NOT UNIX)
#     SET(CMAKE_C_FLAGS_DEBUG "/MTd /Z7 /Od")
#     SET(CMAKE_C_FLAGS_RELEASE "/MT /O2")
# ENDIF(NOT UNIX)

#We are using the newest version of boost
IF(NOT UNIX)
    #We need to use this definition for some reason:
    #http://stackoverflow.com/questions/6468681/problem-linking-to-boost-library-with-cmake-generated-project-file-for-msvc9
    ADD_DEFINITIONS( -DBOOST_ALL_NO_LIB )
    #Not sure if static library linkage is needed
    SET(Boost_USE_STATIC_LIBS ON)
ENDIF(NOT UNIX)

FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    )

ADD_SUBDIRECTORY(src)
