cmake_minimum_required(VERSION 3.21)

project(Mangrove)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml Svg)
include(InstallRequiredSystemLibraries)
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

add_executable(
        Mangrove
        src/main.cpp
        src/Plan/Plan.cpp
        src/Stand/Stand.cpp
        src/Constraint/AConstraint.cpp
        src/Constraint/Space/Space.cpp
        src/Constraint/Electricity/Electricity.cpp
        src/Constraint/Compatibility/Compatibility.cpp)

target_include_directories(Mangrove PRIVATE
        src/Plan
        src/Stand
        src/Constraint
        src/Constraint/Space
        src/Constraint/Electricity
        src/Constraint/Compatibility
)