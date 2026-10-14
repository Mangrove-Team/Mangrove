# Mangrove Project
# Sanitizers.cmake
# Description: Project-wide sanitizers and fuzzing instrumentation.
#   -DMANGROVE_SANITIZERS="address;undefined"   ASan + UBSan (GCC or Clang)
#   -DMANGROVE_FUZZ=ON                          libFuzzer targets in tests/fuzz (Clang only)
# Must be included before any target is created.

set(MANGROVE_SANITIZERS "" CACHE STRING "Sanitizers to build with, e.g. \"address;undefined\"")
option(MANGROVE_FUZZ "Build the libFuzzer targets (Clang only)" OFF)

if(MANGROVE_FUZZ)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "MANGROVE_FUZZ needs Clang (libFuzzer). Configure with -DCMAKE_CXX_COMPILER=clang++.")
    endif()
    # Coverage feedback for libFuzzer in every library the fuzz targets link.
    add_compile_options(-fsanitize=fuzzer-no-link)
    if(NOT MANGROVE_SANITIZERS)
        set(MANGROVE_SANITIZERS "address;undefined")
    endif()
endif()

if(MANGROVE_SANITIZERS)
    if(MSVC)
        if(NOT MANGROVE_SANITIZERS STREQUAL "address")
            message(FATAL_ERROR "MSVC only supports MANGROVE_SANITIZERS=address")
        endif()
        add_compile_options(/fsanitize=address)
    else()
        list(JOIN MANGROVE_SANITIZERS "," mangrove_sanitizers)
        # Any finding aborts the process, so a test that triggers one fails.
        add_compile_options(-fsanitize=${mangrove_sanitizers} -fno-sanitize-recover=all
                            -fno-omit-frame-pointer)
        add_link_options(-fsanitize=${mangrove_sanitizers})
    endif()
    message(STATUS "Mangrove: sanitizers enabled: ${MANGROVE_SANITIZERS}")
endif()
