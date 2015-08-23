# COMEDI for CMake (you're welcome)
cmake_minimum_required(VERSION 2.8)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "-std=c++11 /usr/local/lib/libcomedi.so -lm")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "-std=c++0x /usr/local/lib/libcomedi.so -lm")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

project(beast-tracker)

SET(COMEDI_INSTALL "/usr/local/")
FIND_FILE(COMEDILIB_FOUND "comedilib.h" ${COMEDI_INSTALL}/include)
SET(COMEDI_INCLUDE_DIRS "${COMEDI_INSTALL}/include")
SET(COMEDI_LINK_DIRS "${COMEDI_INSTALL}/lib")
SET(COMEDI_LIBS "comedi")
FIND_LIBRARY(COMEDI_LIBRARY NAMES comedilib comedi HINTS "${COMEDI_LINK_DIRS}")


find_package( OpenCV REQUIRED )
add_executable( beast-tracker beast-tracker.cpp )
target_link_libraries( beast-tracker ${OpenCV_LIBS} )

find_library( FLYCAPTURE2 flycapture)
target_link_libraries( beast-tracker ${FLYCAPTURE2})

target_link_libraries( beast-tracker ${COMEDI_LIBRARY} )

