add_subdirectory(utils)

add_executable(TimerTest TimerTest.cpp)
target_link_libraries(TimerTest Timer Utils gtest_main)