# code by </MATRIX>@Neod Anderjon

cmake_minimum_required(VERSION 3.9)
project(opencv_demo)

set(CMAKE_CXX_STANDARD 11)

add_executable(opencv_demo main.cpp)

find_package(OpenCV REQUIRED)
target_link_libraries(opencv_demo ${OpenCV_LIBS})
