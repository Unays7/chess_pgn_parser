cmake_minimum_required(VERSION 3.25.0)
project(PGN_PARSER LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCES src/main.cpp)

# Compile the executable
add_executable(PGN_PARSER src/main.cpp)