cmake_minimum_required(VERSION 3.10)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

project(Car_Controller)

set(MCU atmega32)
set(F_CPU 16000000UL)
set(AVR_PROGRAMMER usbasp)

include_directories(.)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)

add_compile_options(
    -mmcu=${MCU}
    -DF_CPU=${F_CPU}
    -Os
    -Wall
    -ffunction-sections
    -fdata-sections
)

add_link_options(
    -mmcu=${MCU}
    -Wl,--gc-sections
)

set(SOURCES
    main.cpp
    carcontrol.cpp
)

add_executable(${PROJECT_NAME}.elf ${SOURCES})

add_custom_command(
    TARGET ${PROJECT_NAME}.elf
    POST_BUILD
    COMMAND avr-objcopy -j .text -j .data -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND avr-size --format=avr --mcu=${MCU} ${PROJECT_NAME}.elf
)

add_custom_target(upload
    COMMAND avrdude -c ${AVR_PROGRAMMER} -p ${MCU} -U flash:w:${PROJECT_NAME}.hex
    DEPENDS ${PROJECT_NAME}.elf
)

add_custom_target(set_fuses
    COMMAND avrdude -c ${AVR_PROGRAMMER} -p ${MCU} -U lfuse:w:0xff:m -U hfuse:w:0xd9:m
)