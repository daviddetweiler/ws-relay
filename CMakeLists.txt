cmake_minimum_required(VERSION 3.10)

project(ws-relay)

find_package(Boost REQUIRED)

add_executable(relay main.cpp)
target_include_directories(relay PRIVATE websocketpp)
target_include_directories(relay PRIVATE ${Boost_INCLUDE_DIRS})
