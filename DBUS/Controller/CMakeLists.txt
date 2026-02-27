cmake_minimum_required(VERSION 3.16)


project(CloudController VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core DBus)

add_executable(CloudController
    main.cpp
    controller.h
    controller.cpp
)

target_link_libraries(CloudController PRIVATE Qt6::Core Qt6::DBus)