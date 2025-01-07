cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(rolling-ipc VERSION 1.0)

add_executable(testclient)

target_sources(testclient PUBLIC testclient/client.cpp)

add_executable(testserver)

target_sources(testserver PUBLIC testserver/server.cpp)