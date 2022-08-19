add_subdirectory(utils)

add_executable(TimeUtilsTest TimeUtilsTest.cpp)
target_link_libraries(TimeUtilsTest TimeUtils gtest_main)

add_executable(TimerTest TimerTest.cpp)
target_link_libraries(TimerTest Timer TimeUtils gtest_main)