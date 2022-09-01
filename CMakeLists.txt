cmake_minimum_required(VERSION 3.23)
project(simple_tcp_server_client)

set(CMAKE_CXX_STANDARD 23)

add_executable(client src/client.cpp)
add_executable(server src/server.cpp)
