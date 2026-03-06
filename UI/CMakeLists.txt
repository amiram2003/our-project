cmake_minimum_required(VERSION 3.16)

project(CarGUI LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 REQUIRED COMPONENTS Gui Qml Quick)

add_executable(CarGUI 
    main.cpp 
    vehicledata.cpp 
    vehicledata.h 
    resources.qrc
)

target_link_libraries(CarGUI PRIVATE 
    Qt6::Gui 
    Qt6::Qml 
    Qt6::Quick
)