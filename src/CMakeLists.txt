add_subdirectory(utils)

add_library(Benchmark Benchmark.cpp)
target_link_libraries(Benchmark PUBLIC Utils)

add_library(${PROJECT_NAME}::Benchmark ALIAS Benchmark)

add_library(Timer Timer.cpp)
target_link_libraries(Timer PUBLIC Utils)

add_library(${PROJECT_NAME}::Timer ALIAS Timer)