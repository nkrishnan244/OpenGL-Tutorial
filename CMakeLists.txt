cmake_minimum_required(VERSION 3.5.1)

set(This RoboticArm)

project(${This})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS}) 

add_executable(${This} main.cpp)

target_link_libraries(${This} ${OPENGL_LIBRARIES} ${GLFW_STATIC_LIBRARIES} ${GLEW_LIBRARIES})