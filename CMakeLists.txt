cmake_minimum_required(VERSION 3.21)
project(timed VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/include/)

add_subdirectory(src)

add_subdirectory(examples)

