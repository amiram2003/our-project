cmake_minimum_required(VERSION 3.16)
project(CloudService VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core DBus Network)

add_executable(CloudService
    main.cpp
    cloud.h
    cloud.cpp
)

target_link_libraries(CloudService PRIVATE Qt6::Core Qt6::DBus Qt6::Network)