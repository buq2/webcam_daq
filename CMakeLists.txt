cmake_minimum_required (VERSION 2.6)
PROJECT(cdaq C CXX)

#Make sure that all projects have same compilation type
IF (NOT DEFINED CDAQ_BUILD_TYPE)
    SET(CDAQ_BUILD_TYPE  DEBUG)
ENDIF (NOT DEFINED CDAQ_BUILD_TYPE)
SET(CMAKE_BUILD_TYPE ${CDAQ_BUILD_TYPE})

#Either static or dynamic libraries
SET(CDAQ_LIBRARY_TYPE STATIC)
#If static, headers must have certain definitions
IF(${CDAQ_LIBRARY_TYPE} MATCHES "STATIC")
    ADD_DEFINITIONS( -DCDAQ_STATIC_LIBRARIES )
ENDIF()

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

FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    )
LINK_DIRECTORIES(
    ${Boost_LIBRARY_DIRS}
    )
FIND_PACKAGE(OpenCV REQUIRED
            )
INCLUDE_DIRECTORIES(
    ${OpenCV_INCLUDE_DIRS}
    )    
ADD_SUBDIRECTORY(src)
