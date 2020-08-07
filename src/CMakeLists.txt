cmake_minimum_required(VERSION 3.0)
project(apriltag_example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -std=c++11 ")

find_package(OpenCV REQUIRED)

#find_package( apriltag REQUIRED)
find_package(PkgConfig)
pkg_search_module(apriltag REQUIRED apriltag)
set(apriltag_INCLUDE_DIRS "${apriltag_INCLUDE_DIRS}/apriltag")
link_directories(${apriltag_LIBDIR})


include_directories(include
  ${apriltag_INCLUDE_DIRS}
)

add_executable(opencv_demo
opencv_demo.cc
)

add_executable(apriltag_demo
apriltag_demo.c
)

target_link_libraries(opencv_demo ${apriltag_LIBRARIES} ${OpenCV_LIBS})

target_link_libraries(apriltag_demo ${apriltag_LIBRARIES})
