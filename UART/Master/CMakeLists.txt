cmake_minimum_required(VERSION 3.16)
project(MasterProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core SerialPort) 

add_executable(MasterApp
    main.cpp
    master.cpp
    master.h
)

target_link_libraries(MasterApp PRIVATE Qt6::Core Qt6::SerialPort)