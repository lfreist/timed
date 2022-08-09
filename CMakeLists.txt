cmake_minimum_required(VERSION 3.22)
project(timed VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_library(Benchmark include/Benchmark.h src/Benchmark.cpp)

add_library(Timer include/Timer.h src/Timer.cpp)
