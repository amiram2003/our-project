cmake_minimum_required(VERSION 3.16)

project(SystemController VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core DBus)

add_executable(SystemController
    main.cpp
    controller.h
    controller.cpp
)

target_link_libraries(SystemController PRIVATE Qt6::Core Qt6::DBus)