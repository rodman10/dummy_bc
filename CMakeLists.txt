cmake_minimum_required(VERSION 3.9)
project(block_chain)

set(CMAKE_CXX_STANDARD 11)
set(HEADER_DIR ${PROJECT_SOURCE_DIR}/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${HEADER_DIR})

add_subdirectory(src)
add_subdirectory(test)

