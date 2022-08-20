if (NOT TARGET Statistics)
add_library(Statistics Statistics.cpp)
endif()

if (NOT TARGET ${PROJECT_NAME}::Statistics)
add_library(${PROJECT_NAME}::Statistics ALIAS Statistics)
endif()