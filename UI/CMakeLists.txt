cmake_minimum_required(VERSION 3.16)

project(CarGUI LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Added Core and Network to the find_package list
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick DBus Network)

add_executable(CarGUI 
    main.cpp 
    vehicledata.cpp 
    vehicledata.h 
    dbusreader.cpp
    dbusreader.h
    resources.qrc
)

# Linked the new components to your executable
target_link_libraries(CarGUI PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::DBus
    Qt6::Network
)