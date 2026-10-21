# Mangrove Project
# VcpkgBootstrap.cmake
# Description: Selects the vcpkg toolchain before project() so Qt is installed
#              automatically from vcpkg.json. Must be included before project().
#
# Lookup order:
#   1. CMAKE_TOOLCHAIN_FILE already set (preset / command line) -> untouched
#   2. $ENV{VCPKG_ROOT} -> use that vcpkg installation
#   3. Otherwise clone vcpkg into external/vcpkg at the manifest baseline

option(MANGROVE_USE_VCPKG "Install dependencies (Qt) through vcpkg instead of using the system Qt" OFF)

if(NOT MANGROVE_USE_VCPKG OR CMAKE_TOOLCHAIN_FILE)
    return()
endif()

if(DEFINED ENV{VCPKG_ROOT} AND EXISTS "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    set(_mangrove_vcpkg_root "$ENV{VCPKG_ROOT}")
else()
    set(_mangrove_vcpkg_root "${CMAKE_CURRENT_LIST_DIR}/../external/vcpkg")
    cmake_path(NORMAL_PATH _mangrove_vcpkg_root)

    if(NOT EXISTS "${_mangrove_vcpkg_root}/scripts/buildsystems/vcpkg.cmake")
        find_package(Git REQUIRED)
        file(READ "${CMAKE_CURRENT_LIST_DIR}/../vcpkg.json" _mangrove_manifest)
        string(JSON _mangrove_baseline GET "${_mangrove_manifest}" builtin-baseline)

        message(STATUS "vcpkg not found, cloning it into ${_mangrove_vcpkg_root}")
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" clone https://github.com/microsoft/vcpkg.git "${_mangrove_vcpkg_root}"
            RESULT_VARIABLE _mangrove_git_result)
        if(NOT _mangrove_git_result EQUAL 0)
            message(FATAL_ERROR "Failed to clone vcpkg")
        endif()
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" -C "${_mangrove_vcpkg_root}" checkout --quiet "${_mangrove_baseline}"
            RESULT_VARIABLE _mangrove_git_result)
        if(NOT _mangrove_git_result EQUAL 0)
            message(FATAL_ERROR "Failed to checkout vcpkg baseline ${_mangrove_baseline}")
        endif()
    endif()
endif()

# vcpkg.cmake bootstraps the vcpkg executable itself if it is missing.
set(CMAKE_TOOLCHAIN_FILE "${_mangrove_vcpkg_root}/scripts/buildsystems/vcpkg.cmake"
    CACHE FILEPATH "vcpkg toolchain file")
message(STATUS "Using vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
