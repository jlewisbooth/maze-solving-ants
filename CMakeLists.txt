cmake_minimum_required(VERSION 3.10)

add_definitions(-std=c++11)

set(CMAKE_BUILD_TYPE Debug)

project(ACO VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

configure_file(main.h.in main.h)

add_executable (ACO main.cpp)

add_subdirectory(maze_builder)
add_subdirectory(aco_lib)

target_link_libraries(ACO PUBLIC MazeBuilder)
target_link_libraries(ACO PUBLIC ACOLIB)

target_include_directories(ACO PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/maze_builder"
                           "${PROJECT_SOURCE_DIR}/aco_lib"
                           )