cmake_minimum_required(VERSION 3.24)
project(GlslTest)
set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)

FILE(GLOB_RECURSE SRC_LIST ${CMAKE_CURRENT_LIST_DIR}/src/*.*)

add_executable(GlslTest ${SRC_LIST})

add_subdirectory(libs/glad)

add_subdirectory(libs/nlohmann_json)

add_subdirectory(libs/glfw)

target_link_libraries(GlslTest ${OPENGL_LIBRARY} glad nlohmann_json::nlohmann_json glfw)
include_directories(GlslTest ${OPENGL_INCLUDE_DIR} src)
