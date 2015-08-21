# COMEDI for CMake (you're welcome)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11 /usr/local/lib/libcomedi.so -lm")
project(beast-tracker)

SET(COMEDI_INSTALL "/usr/local/src/new")
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

