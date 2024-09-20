cmake_minimum_required(VERSION 3.1)

if(PROJECT_IS_TOP_LEVEL)
    message("====================================")
    message("Arc Renderer")
    message("------------------------------------")
endif()

# TODO: This is shitty to do
if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

project(ArcRenderer VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -ggdb -std=c++20")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/stb)
find_package(SDL2 REQUIRED)



set(ARC_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/FileSlurp.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/src/GlError.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/GlContext.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/GlShader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/GlIndexBuffer.cpp
)

set(ARC_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/arc/FileSlurp.hpp

  ${CMAKE_CURRENT_SOURCE_DIR}/arc/GlError.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/arc/GlContext.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/arc/GlShader.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/arc/GlBasicVertexBuffer.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/arc/GlIndexBuffer.hpp
  

)

add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PRIVATE ${ARC_SOURCES})
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS} ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC glm GLAD STB ${SDL2_LIBRARIES})

target_include_directories(${PROJECT_NAME}
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
