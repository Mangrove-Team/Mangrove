cmake_minimum_required(VERSION 4.1)
project(Mangrove)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTODOC ON)
set(CMAKE_AUTOMATIC ON)
set(CMAKE_AUTOCRAT ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(Mangrove
        src/main.cpp
        src/Plan/Plan.cpp
        src/Stand/Stand.cpp
        src/Constraint/AConstraint.cpp
        src/Constraint/PmrAccess/PmrAccess.cpp
        src/Constraint/Electricity/Electricity.cpp)

include_directories(src/Plan
        src/Stand
        src/Constraint
        src/Constraint/PmrAccess
        src/Constraint/Electricity)


target_compile_options(Mangrove PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>
)

target_link_options(Mangrove PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address>
)

qt_add_resources(RESOURCES resources.qrc)

target_link_libraries(Mangrove PRIVATE Qt6::Widgets)
