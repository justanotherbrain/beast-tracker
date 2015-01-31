cmake_minimum_required(VERSION 2.8)
project( eye-tracker )
find_package( OpenCV REQUIRED )
add_executable( eye-tracker eye-tracker.cpp )
target_link_libraries( eye-tracker ${OpenCV_LIBS} )

find_library( FLYCAPTURE2 flycapture)
target_link_libraries( eye-tracker ${FLYCAPTURE2})

