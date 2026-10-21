# Mangrove Project
# CompilerWarnings.cmake
# Description: mangrove_enable_warnings(<target> [STRICT])
#              STRICT adds conversion/shadowing checks; use it for code that
#              does not go through Qt's generated sources (moc, qmlcachegen).

option(MANGROVE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

function(mangrove_enable_warnings target)
    cmake_parse_arguments(PARSE_ARGV 1 arg "STRICT" "" "")

    if(MSVC)
        set(flags /W4 /permissive-)
        if(MANGROVE_WARNINGS_AS_ERRORS)
            list(APPEND flags /WX)
        endif()
    else()
        set(flags -Wall -Wextra -Wpedantic -Wnon-virtual-dtor -Woverloaded-virtual)
        if(arg_STRICT)
            list(APPEND flags -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast)
        endif()
        if(MANGROVE_WARNINGS_AS_ERRORS)
            list(APPEND flags -Werror)
        endif()
    endif()

    target_compile_options(${target} PRIVATE ${flags})
endfunction()
