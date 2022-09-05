add_subdirectory(utils)

if (NOT TARGET TimeUtils)
add_library(TimeUtils TimeUtils.cpp)
endif()

if (NOT TARGET ${PROJECT_NAME}::TimeUtils)
add_library(${PROJECT_NAME}::TimeUtils ALIAS TimeUtils)
endif()

if (NOT TARGET Benchmark)
add_library(Benchmark Benchmark.cpp)
target_link_libraries(Benchmark PUBLIC TimeUtils Statistics)
endif()

if (NOT TARGET ${PROJECT_NAME}::Benchmark)
    add_library(${PROJECT_NAME}::Benchmark ALIAS Benchmark)
endif()

if (NOT TARGET Timer)
add_library(Timer Timer.cpp)
target_link_libraries(Timer PUBLIC TimeUtils Statistics)
endif()

if (NOT TARGET ${PROJECT_NAME}::Timer)
add_library(${PROJECT_NAME}::Timer ALIAS Timer)
endif()
