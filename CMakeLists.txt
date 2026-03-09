cmake_minimum_required(VERSION 3.21)

project(Mangrove)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml)
include(InstallRequiredSystemLibraries)

add_executable(Mangrove
        src/main.cpp
        src/Plan/Plan.cpp
        src/Stand/Stand.cpp
        src/Constraint/AConstraint.cpp
        src/Constraint/PmrAccess/PmrAccess.cpp
        src/Constraint/Electricity/Electricity.cpp
        src/Constraint/IConstraint.hpp
)

target_link_libraries(Mangrove PRIVATE Qt6::Quick)

qt_add_qml_module(Mangrove
        URI "MangroveApp"
        VERSION 1.0
        QML_FILES
        src/style/Main.qml
        src/style/Map.qml
        src/style/ListStand.qml
        src/style/Stand.qml
)

add_custom_command(TARGET Mangrove POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:Mangrove>/resources)
target_include_directories(Mangrove PRIVATE
        src/Plan
        src/Stand
        src/Constraint
        src/Constraint/PmrAccess
        src/Constraint/Electricity
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources
        DESTINATION .)
target_compile_options(Mangrove PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>
)

install(TARGETS Mangrove
        DESTINATION .
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll")
target_link_options(Mangrove PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address>
)

set(CPACK_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME "Mangrove")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
include(CPack)
