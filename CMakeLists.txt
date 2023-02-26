cmake_minimum_required(VERSION 3.24)
project(GlslTest)
set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(nlohmann_json 3.11.2 REQUIRED)

FILE(GLOB_RECURSE SRC_LIST ${CMAKE_CURRENT_LIST_DIR}/src/*.*)

add_executable(GlslTest ${SRC_LIST})

target_link_libraries(GlslTest ${GLFW3_LIBRARY} ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} nlohmann_json::nlohmann_json)
include_directories(GlslTest ${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} src)
