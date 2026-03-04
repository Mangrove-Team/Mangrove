cmake_minimum_required(VERSION 3.21)

project(Mangrove)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(InstallRequiredSystemLibraries)

add_executable(Mangrove
        source/Main.cpp
)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)

target_link_libraries(Mangrove PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        SFML::Audio
)

add_custom_command(TARGET Mangrove POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:Mangrove>/resources
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/resources
        DESTINATION .
)

install(TARGETS Mangrove
        DESTINATION .
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
)

set(CPACK_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME "Mangrove")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
include(CPack)
