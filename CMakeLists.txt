cmake_minimum_required(VERSION 3.21)
project(timed VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

set(MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

if (${MAIN_PROJECT})
# ----- google test ----------------------------------------------------------------------------------------------------
    add_subdirectory(third_party/google_test)
    include_directories(third_party/google_test/googletest/include)
endif()


include_directories(${PROJECT_SOURCE_DIR}/include/)

add_subdirectory(src)

if (${MAIN_PROJECT})
    add_subdirectory(examples)
    add_subdirectory(test)
endif()
