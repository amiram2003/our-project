cmake_minimum_required(VERSION 3.16)
project(MockCANTool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(MockCANTool 
    main.cpp 
    canmock.cpp 
    canmock.h
)

target_link_libraries(MockCANTool PRIVATE Qt6::Core)