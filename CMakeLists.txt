# Mangrove Project - root
# Builds the desktop app (logiciel/) from the repository root, so the CI, the
# pre-commit clang-tidy hook (-p=build) and the IDE all share one build/ folder.
# logiciel/ stays buildable on its own (cmake -S logiciel -B logiciel/build).

cmake_minimum_required(VERSION 3.25)

# vcpkg (MANGROVE_USE_VCPKG=ON) must pick its toolchain before project(), and
# read the manifest from logiciel/ instead of the root.
set(VCPKG_MANIFEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/logiciel CACHE PATH "vcpkg manifest directory")
include(logiciel/cmake/VcpkgBootstrap.cmake)

project(MangroveRoot LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Must be called here too, so ctest finds the tests from build/.
enable_testing()

add_subdirectory(logiciel)
