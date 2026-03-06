cmake_minimum_required(VERSION 3.16)
project(MangroveApp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt6 COMPONENTS Core Gui Quick Qml QuickControls2 REQUIRED)

qt_policy(SET QTP0004 NEW)
qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(MangroveApp
        src/main.cpp
        src/Plan/Plan.cpp
        src/Stand/Stand.cpp
        src/Constraint/AConstraint.cpp
        src/Constraint/PmrAccess/PmrAccess.cpp
        src/Constraint/Electricity/Electricity.cpp
        src/DrawingCanvas/DrawingCanvasBackend.cpp
)

qt_add_qml_module(MangroveApp
        URI Mangrove
        VERSION 1.0
        QML_FILES
        qml/main.qml
        RESOURCES resources.qrc
)

target_link_libraries(MangroveApp PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::Qml
        Qt6::QuickControls2
)

target_include_directories(MangroveApp PRIVATE
        src/Plan
        src/Stand
        src/Constraint
        src/Constraint/PmrAccess
        src/Constraint/Electricity
        src/DrawingCanvas
        src/UI
)