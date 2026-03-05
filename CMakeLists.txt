cmake_minimum_required(VERSION 4.1)
project(Mangrove)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml)

add_executable(Mangrove
        src/main.cpp
        src/Plan/Plan.cpp
        src/Stand/Stand.cpp
        src/Constraint/AConstraint.cpp
        src/Constraint/PmrAccess/PmrAccess.cpp
        src/Constraint/Electricity/Electricity.cpp
)

qt_add_qml_module(Mangrove
        URI "MangroveApp"
        VERSION 1.0
        QML_FILES
        src/style/Main.qml
        src/style/Map.qml
)

target_include_directories(Mangrove PRIVATE
        src/Plan
        src/Stand
        src/Constraint
        src/Constraint/PmrAccess
        src/Constraint/Electricity
)

target_compile_options(Mangrove PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>
)

target_link_options(Mangrove PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address>
)

target_link_libraries(Mangrove PRIVATE Qt6::Quick)