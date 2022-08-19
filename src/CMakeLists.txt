add_subdirectory(utils)

add_library(TimeUtils TimeUtils.cpp)
add_library(${PROJECT_NAME}::TimeUtils ALIAS TimeUtils)

add_library(Benchmark Benchmark.cpp)
target_link_libraries(Benchmark PUBLIC TimeUtils Statistics)

add_library(${PROJECT_NAME}::Benchmark ALIAS Benchmark)

add_library(Timer Timer.cpp)
target_link_libraries(Timer PUBLIC TimeUtils Statistics)

add_library(${PROJECT_NAME}::Timer ALIAS Timer)