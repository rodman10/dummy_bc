cmake_minimum_required(VERSION 3.9)
project(block_chain)

set(CMAKE_CXX_STANDARD 11)
include_directories(${HEADER_DIR})

add_executable(BlockChainTest DatastructTest.cpp)

add_executable(RequestTest RequestTest.cpp)
target_link_libraries(RequestTest http_request http_parser jsoncpp )

add_executable(httpd httpd.cpp handler.cpp)
target_link_libraries(httpd http_request http_parser pthread jsoncpp block_chain)