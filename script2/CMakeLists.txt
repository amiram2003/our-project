cmake_minimum_required(VERSION 3.16)

project(VibrationSimulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON) 

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(VibrationSimulator
    main.cpp
    vibrationsimulator.h
    vibrationsimulator.cpp
)

target_link_libraries(VibrationSimulator PRIVATE Qt6::Core)