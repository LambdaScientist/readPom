cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(PomParser)

include_directories("${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include3P")

# add the executable
add_executable(pomRead main.cpp)