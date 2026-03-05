# Advanced Driver Security and Assistance System (ADSAS)

This repository contains several sub-projects that make up the ADSAS system. Each sub-project is built using CMake and requires Qt6 components.

## Prerequisites

- **CMake**: Version 3.16 or higher
- **C++ Compiler**: Supporting C++17
- **Qt6**: Required components include Core, DBus, Network, SerialBus, SerialPort, Gui, Qml, and Quick.
- **Google Test**: Used for unit testing (optional for building the main applications).

## Project Structure

- `CANCoummincation`: Handles CAN bus communication.
- `Cloud`: Service for cloud connectivity.
- `Controller`: Main logic controller using DBus.
- `Serial`: Contains Master and Slave applications for serial communication.
- `UI`: The graphical user interface built with Qt Quick/QML.

## How to Build Every Repo

Each sub-project follows the standard CMake build process. You can build them individually as follows:

### e.g. Controller
```bash
cd Controller
cmake -B build && cmake --build build
```


## Running Tests

The project uses **Google Test** for unit testing. After building a project, you can run the generated test executables (if any) or use `ctest` within the build directory.

```bash
cd <ProjectName>/build
ctest
```
