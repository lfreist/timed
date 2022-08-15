add_library(Utils Statistics.cpp TimeUtils.cpp)

add_library(${PROJECT_NAME}::Utils ALIAS Utils)