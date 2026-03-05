cmake_minimum_required(VERSION 3.16)
project(CAN LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# البحث عن المكتبات الأساسية والـ SerialBus (مهم للـ CAN)
find_package(Qt6 REQUIRED COMPONENTS Core SerialBus)

# محاولة البحث عن Connectivity بس مش هنوقف الـ Build لو مجتش
find_package(Qt6 COMPONENTS Connectivity QUIET)

add_executable(Can_ATest main.cpp canhandler.cpp canhandler.h)

target_link_libraries(A PRIVATE Qt6::Core Qt6::SerialBus)

# لو لقى Connectivity يربطها
if(Qt6Connectivity_FOUND)
    target_link_libraries(A PRIVATE Qt6::Connectivity)
endif()