cmake_minimum_required(VERSION 3.1)
project(window)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -ggdb")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} test.cpp)

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../ 
  ${CMAKE_CURRENT_SOURCE_DIR}/ArcRenderer
)
target_link_libraries(${PROJECT_NAME} PRIVATE ArcRenderer)
